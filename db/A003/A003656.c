/*
 * A003656  Discriminants of real quadratic fields with unique factorization.
 * https://oeis.org/A003656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: d runs over fundamental discriminants d > 1 with at most 2 distinct prime factors
   (genus theory). For each one, count all R reduced indefinite forms of discriminant d,
   and walk the principal cycle of length L under the rho map. The class number is 1
   iff R = L when -f0 lies on the principal cycle, and R = 2L when it does not. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *spf = NULL;
static uint64_t spf_lim = 0;

static void ensure_sieve(uint64_t x) {
    if (x <= spf_lim) return;
    uint64_t L = spf_lim * 2;
    if (L < x) L = x;
    if (L < 65536) L = 65536;
    free(spf);
    spf = (uint32_t *)malloc((size_t)(L + 1) * sizeof(uint32_t));
    for (uint64_t i = 0; i <= L; i++) spf[i] = 0;
    for (uint64_t i = 2; i <= L; i++)
        if (spf[i] == 0)
            for (uint64_t j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
    spf_lim = L;
}

static uint64_t isqrt_u64(uint64_t x) {
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x) {
        uint64_t step = 1;
        while ((r + step * 2) * (r + step * 2) <= x) step *= 2;
        r += step;
    }
    return r;
}

/* fundamental discriminant with at most 2 distinct prime factors */
static bool candidate(uint64_t d) {
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
        uint64_t p = spf[m];
        m /= p;
        if (m % p == 0) return false;
        t++;
    }
    return t <= 2;
}

/* number of divisors m of N with lo <= 2m <= hi */
static uint64_t count_divs(uint64_t N, int64_t lo, int64_t hi) {
    uint64_t pr[20];
    int ex[20], k = 0;
    while (N > 1) {
        uint64_t p = spf[N];
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

static bool class_one(uint64_t du) {
    int64_t d = (int64_t)du;
    int64_t s = (int64_t)isqrt_u64(du);
    /* count reduced forms: 1<=b<=s, s+1-b <= 2|a| <= s+b */
    uint64_t R = 0;
    for (int64_t b = 1; b <= s; b++) {
        if (((b - d) % 2) != 0) continue;
        uint64_t N = (uint64_t)((d - b * b) / 4);
        R += 2 * count_divs(N, s + 1 - b, s + b);
    }
    /* principal cycle */
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

static uint64_t *terms = NULL;
static uint64_t nterms = 0, cap = 0, next_d = 5;

seq_t a(idx_t n) {
    while (nterms < n) {
        uint64_t d = next_d++;
        ensure_sieve(d);
        if (!candidate(d)) continue;
        if (!class_one(d)) continue;
        if (nterms == cap) {
            cap = cap ? cap * 2 : 1024;
            terms = (uint64_t *)realloc(terms, (size_t)cap * sizeof(uint64_t));
        }
        terms[nterms++] = d;
    }
    return (seq_t)terms[n - 1];
}
