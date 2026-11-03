/*
 * A003162  A binomial coefficient summation.
 * https://oeis.org/A003162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (binomial(n,k)-binomial(n,k-1))^3 /
 *            binomial(n,floor(n/2)), computed by 4*n*(n+1)^2*p(n)*a(n) =
 *            2*n*(1372*n^4 - 3633*n^3 - 7455*n^2 + 21934*n - 8448)*a(n-1) +
 *            (12740*n^6 - 90867*n^5 + 195310*n^4 - 13277*n^3 - 452690*n^2 +
 *            528384*n - 174960)*a(n-2) + 8*(n-2)*(686*n^4 -...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 945 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

static seq_t a3162_big(idx_t n) {
    seq_t A0 = 1, A1 = 1, A2 = 1, A3 = 3;
    idx_t m;
    if (n == 0) return A0;
    if (n == 1) return A1;
    if (n == 2) return A2;
    if (n == 3) return A3;
    for (m = 4; m <= n; m++) {
        seq_t N = (seq_t)m;
        seq_t M1 = N + 1;
        seq_t N2 = N * N, N3 = N2 * N, N4 = N3 * N, N5 = N4 * N, N6 = N5 * N;
        seq_t p = (N3 * 196 + N * 530 + 528) - N2 * 819;
        seq_t p1 = (M1 * M1 * M1 * 196 + M1 * 530 + 528) - M1 * M1 * 819;
        seq_t q = (N4 * 1372 + N * 21934) - (N3 * 3633 + N2 * 7455 + 8448);
        seq_t rr = (N6 * 12740 + N4 * 195310 + N * 528384)
                   - (N5 * 90867 + N3 * 13277 + N2 * 452690 + 174960);
        seq_t s = (N4 * 686 + N2 * 1176 + N * 6543) - (N3 * 3010 + 4725);
        seq_t num = N * 2 * q * A3 + rr * A2 + (N - 2) * 8 * s * A1;
        seq_t sub = (N - 3) * (N - 3) * (N - 2) * 16 * p1 * A0;
        seq_t den = N * 4 * M1 * M1 * p;
        seq_t next = (num - sub) / den;
        A0 = A1;
        A1 = A2;
        A2 = A3;
        A3 = next;
    }
    return A3;
}

static int a3162_is_big(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 200; i++) x = (seq_t)(x * 2);
    return x != 0;
}

#define A3162_LIMBS 16
#define A3162_FIXED_MAX 150

/* acc += x * c */
static void a3162_muladd(uint32_t *acc, const uint32_t *x, uint64_t c) {
    unsigned __int128 carry = 0;
    int i;
    for (i = 0; i < A3162_LIMBS; i++) {
        unsigned __int128 cur = (unsigned __int128)x[i] * c + acc[i] + carry;
        acc[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

/* acc -= y */
static void a3162_subn(uint32_t *acc, const uint32_t *y) {
    uint64_t borrow = 0;
    int i;
    for (i = 0; i < A3162_LIMBS; i++) {
        uint64_t s = (uint64_t)y[i] + borrow;
        uint64_t v = acc[i];
        if (v >= s) { acc[i] = (uint32_t)(v - s); borrow = 0; }
        else { acc[i] = (uint32_t)(v + ((uint64_t)1 << 32) - s); borrow = 1; }
    }
}

/* x /= d */
static void a3162_divn(uint32_t *x, uint64_t d) {
    unsigned __int128 rem = 0;
    int i;
    for (i = A3162_LIMBS - 1; i >= 0; i--) {
        unsigned __int128 cur = (rem << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static seq_t a3162_fixed(idx_t n) {
    uint32_t A[4][A3162_LIMBS];
    uint32_t num[A3162_LIMBS], neg[A3162_LIMBS];
    static const uint32_t init[4] = {1, 1, 1, 3};
    idx_t m;
    int i;
    seq_t r = 0;
    if (n > A3162_FIXED_MAX) return 0; /* value cannot fit any fixed width */
    for (i = 0; i < 4; i++) {
        memset(A[i], 0, sizeof(A[i]));
        A[i][0] = init[i];
    }
    for (m = 4; m <= n; m++) {
        long long N = (long long)m, M1 = N + 1;
        long long p = 196 * N * N * N - 819 * N * N + 530 * N + 528;
        long long p1 = 196 * M1 * M1 * M1 - 819 * M1 * M1 + 530 * M1 + 528;
        long long q = 1372 * N * N * N * N - 3633 * N * N * N - 7455 * N * N + 21934 * N - 8448;
        long long N2 = N * N, N3 = N2 * N, N4 = N3 * N, N5 = N4 * N, N6 = N5 * N;
        long long rr = 12740 * N6 - 90867 * N5 + 195310 * N4 - 13277 * N3
                       - 452690 * N2 + 528384 * N - 174960;
        long long s = 686 * N4 - 3010 * N3 + 1176 * N2 + 6543 * N - 4725;
        uint64_t c1 = (uint64_t)(2 * N * q);
        uint64_t c2 = (uint64_t)rr;
        uint64_t c3 = (uint64_t)(8 * (N - 2) * s);
        uint64_t c4 = (uint64_t)(16 * (N - 3) * (N - 3) * (N - 2) * p1);
        uint64_t d = (uint64_t)(4 * N * M1 * M1 * p);
        memset(num, 0, sizeof(num));
        memset(neg, 0, sizeof(neg));
        a3162_muladd(num, A[3], c1);
        a3162_muladd(num, A[2], c2);
        a3162_muladd(num, A[1], c3);
        a3162_muladd(neg, A[0], c4);
        a3162_subn(num, neg);
        a3162_divn(num, d);
        memcpy(A[0], A[1], sizeof(A[0]));
        memcpy(A[1], A[2], sizeof(A[0]));
        memcpy(A[2], A[3], sizeof(A[0]));
        memcpy(A[3], num, sizeof(A[0]));
    }
    {
        const uint32_t *v = (n < 4) ? A[n] : A[3];
        for (i = 15; i >= 0; i--) {
            uint32_t byte = (v[i / 4] >> (8 * (i % 4))) & 255u;
            r = (seq_t)(r * 256 + byte);
        }
    }
    return r;
}

seq_t a(idx_t n) {
    if (a3162_is_big()) return a3162_big(n);
    return a3162_fixed(n);
}
