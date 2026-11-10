/*
 * A071581  A003656(n)-prime(n).
 * https://oeis.org/A071581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A003656(n) - prime(n); A003656 by search over fundamental
 *            discriminants (class number 1 test), prime(n) by sieve
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *A071_spf = NULL;
static uint64_t A071_spf_lim = 0;

static void A071_ensure_sieve(uint64_t x) {
    if (x <= A071_spf_lim) return;
    uint64_t L = A071_spf_lim * 2;
    if (L < x) L = x;
    if (L < 65536) L = 65536;
    free(A071_spf);
    A071_spf = (uint32_t *)malloc((size_t)(L + 1) * sizeof(uint32_t));
    for (uint64_t i = 0; i <= L; i++) A071_spf[i] = 0;
    for (uint64_t i = 2; i <= L; i++)
        if (A071_spf[i] == 0)
            for (uint64_t j = i; j <= L; j += i)
                if (A071_spf[j] == 0) A071_spf[j] = (uint32_t)i;
    A071_spf_lim = L;
}

static uint64_t A071_isqrt(uint64_t x) {
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x) {
        uint64_t step = 1;
        while ((r + step * 2) * (r + step * 2) <= x) step *= 2;
        r += step;
    }
    return r;
}

static bool A071_candidate(uint64_t d) {
    uint64_t m;
    int extra = 0;
    if (d % 4 == 1) m = d;
    else if (d % 4 == 0) {
        m = d / 4;
        if (m % 4 != 2 && m % 4 != 3) return false;
        if (m % 2 == 1) extra = 1;
    } else return false;
    int t = extra;
    while (m > 1) {
        uint64_t p = A071_spf[m];
        m /= p;
        if (m % p == 0) return false;
        t++;
    }
    return t <= 2;
}

static uint64_t A071_count_divs(uint64_t N, int64_t lo, int64_t hi) {
    uint64_t pr[20];
    int ex[20], k = 0;
    while (N > 1) {
        uint64_t p = A071_spf[N];
        int e = 0;
        while (N % p == 0) { N /= p; e++; }
        pr[k] = p; ex[k] = e; k++;
    }
    uint64_t divs[4096];
    int nd = 1;
    divs[0] = 1;
    for (int i = 0; i < k; i++) {
        int cur = nd;
        uint64_t pw = 1;
        for (int e = 1; e <= ex[i]; e++) {
            pw *= pr[i];
            for (int j = 0; j < cur; j++) divs[nd++] = divs[j] * pw;
        }
    }
    uint64_t c = 0;
    for (int j = 0; j < nd; j++) {
        int64_t t = 2 * (int64_t)divs[j];
        if (t >= lo && t <= hi) c++;
    }
    return c;
}

static bool A071_class_one(uint64_t du) {
    int64_t d = (int64_t)du;
    int64_t s = (int64_t)A071_isqrt(du);
    uint64_t R = 0;
    for (int64_t b = 1; b <= s; b++) {
        if (((b - d) % 2) != 0) continue;
        uint64_t N = (uint64_t)((d - b * b) / 4);
        R += 2 * A071_count_divs(N, s + 1 - b, s + b);
    }
    int64_t b0 = (s % 2 == d % 2) ? s : s - 1;
    int64_t a0 = 1, c0 = (b0 * b0 - d) / 4;
    int64_t a = a0, b = b0, c = c0;
    uint64_t L = 0;
    bool negfound = false;
    do {
        int64_t ac = c < 0 ? -c : c;
        int64_t m2 = 2 * ac;
        int64_t r;
        int64_t nb = ((-b) % m2 + m2) % m2;
        if (ac > s) {
            r = nb;
            if (r > ac) r -= m2;
        } else {
            r = s - (((s - nb) % m2) + m2) % m2;
        }
        int64_t na = c, nc = (r * r - d) / (4 * c);
        a = na; b = r; c = nc;
        L++;
        if (a == -a0 && b == b0 && c == -c0) negfound = true;
        if (L > R) return false;
    } while (!(a == a0 && b == b0 && c == c0));
    uint64_t need = negfound ? L : 2 * L;
    return R == need;
}

static uint64_t *A071_terms = NULL;
static uint64_t A071_nterms = 0, A071_cap = 0, A071_next_d = 5;

static uint64_t A071_A003656(uint64_t n) {
    while (A071_nterms < n) {
        uint64_t d = A071_next_d++;
        A071_ensure_sieve(d);
        if (!A071_candidate(d)) continue;
        if (!A071_class_one(d)) continue;
        if (A071_nterms == A071_cap) {
            A071_cap = A071_cap ? A071_cap * 2 : 1024;
            A071_terms = (uint64_t *)realloc(A071_terms, (size_t)A071_cap * sizeof(uint64_t));
        }
        A071_terms[A071_nterms++] = d;
    }
    return A071_terms[n - 1];
}

static uint32_t *A071_primes = NULL;
static uint64_t A071_np = 0;

static void A071_build_primes(uint64_t lim) {
    char *comp = (char *)calloc((size_t)(lim + 1), 1);
    free(A071_primes);
    A071_primes = (uint32_t *)malloc((size_t)(lim / 2 + 2) * sizeof(uint32_t));
    A071_np = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!comp[i]) {
            A071_primes[A071_np++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }
    free(comp);
}

static uint64_t A071_prime(uint64_t n) {
    static uint64_t lim = 0;
    while (A071_np < n) {
        lim = lim ? lim * 2 : 1024;
        A071_build_primes(lim);
    }
    return A071_primes[n - 1];
}

seq_t a(idx_t n) {
    uint64_t t = A071_A003656((uint64_t)n);
    uint64_t p = A071_prime((uint64_t)n);
    return (seq_t)t - (seq_t)p;
}
