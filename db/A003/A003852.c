/*
 * A003852  Order of simple Chevalley group D_6(q), q = prime power.
 * https://oeis.org/A003852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(A000961(n+1),6) where, for q = A000961(n+1), d(q,6) =
 *            q^30 * (q^6-1) * Prod_{i=1..5} (q^(2i)-1) / gcd(4, q^6-1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 2, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *pp_list = NULL;   /* prime powers > 1, increasing */
static uint64_t pp_count = 0;

static void build_prime_powers(uint64_t need) {
    uint64_t limit = 1024;
    while (pp_count < need) {
        uint32_t *spf;
        uint64_t i, j, cnt = 0;
        limit *= 2;
        spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
        for (i = 2; i <= limit; i++) {
            if (spf[i] == 0) {
                for (j = i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
        for (i = 2; i <= limit; i++) {
            uint64_t x = i, p = spf[i];
            while (x % p == 0) x /= p;
            if (x == 1) cnt++;
        }
        free(pp_list);
        pp_list = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
        pp_count = 0;
        for (i = 2; i <= limit; i++) {
            uint64_t x = i, p = spf[i];
            while (x % p == 0) x /= p;
            if (x == 1) pp_list[pp_count++] = i;
        }
        free(spf);
    }
}

/* A000961(k): 1, 2, 3, 4, 5, 7, 8, 9, ... */
static uint64_t a000961(uint64_t k) {
    if (k == 1) return 1;
    if (pp_count < k - 1) build_prime_powers(k - 1);
    return pp_list[k - 2];
}

static seq_t pow_seq(seq_t base, uint64_t e) {
    seq_t r = 1;
    seq_t b = base;
    while (e > 0) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

seq_t a(idx_t n) {
    uint64_t q = a000961((uint64_t)n + 1);
    seq_t qs = (seq_t)q;
    seq_t q2 = qs * qs;
    seq_t result = pow_seq(qs, 30);
    seq_t f;
    int i;
    /* (q^2 - 1); for odd q it is divisible by 8, so divide out gcd = 4 here */
    f = q2 - 1;
    if (q % 2 == 1) f /= 4;
    result *= f;
    {
        seq_t qp = q2;
        for (i = 2; i <= 5; i++) {
            qp *= q2;
            result *= (qp - 1);
        }
    }
    result *= (pow_seq(qs, 6) - 1);
    return result;
}
