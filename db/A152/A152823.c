/*
 * A152823  Largest divisor < n of n^2 + 1, a(1) = 1.
 * https://oeis.org/A152823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest divisor < n of n^2 + 1, a(1)=1
 * Range:     u8 n <= 22, u16 n <= 4246, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* find the largest divisor < n of n^2+1 */
seq_t a(idx_t n)
{
    /* a(1) is defined as 1 by convention */
    if (n <= 1)
        return (seq_t)1;

    /* ---------- initialise primes up to 10000 (sqrt of max n^2+1) ---------- */
    static bool primes_init = false;
    static uint32_t primes[2000];
    static size_t prime_cnt = 0;

    if (!primes_init) {
        /* simple sieve */
        enum { LIM = 10000 };
        static bool is_p[LIM + 1];
        for (int i = 2; i <= LIM; ++i)
            is_p[i] = true;
        for (int p = 2; p * p <= LIM; ++p)
            if (is_p[p])
                for (int q = p * p; q <= LIM; q += p)
                    is_p[q] = false;

        for (int i = 2; i <= LIM; ++i)
            if (is_p[i])
                primes[prime_cnt++] = (uint32_t)i;
        primes_init = true;
    }

    /* ---------- factor m = n^2 + 1 (fits in 64 bits) ---------- */
    uint64_t nn = (uint64_t)n;
    uint64_t m  = nn * nn + 1;

    struct { uint32_t p; int exp; } fac[20];
    size_t fac_cnt = 0;
    uint64_t tmp = m;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            fac[fac_cnt].p  = p;
            fac[fac_cnt].exp = e;
            ++fac_cnt;
        }
    }
    if (tmp > 1) {
        fac[fac_cnt].p  = (uint32_t)tmp;
        fac[fac_cnt].exp = 1;
        ++fac_cnt;
    }

    /* ---------- generate all divisors of m ---------- */
    static seq_t divs[2500];
    size_t cur_sz = 1;
    divs[0] = (seq_t)1;   /* divisor 1 */

    for (size_t f = 0; f < fac_cnt; ++f) {
        uint64_t p = fac[f].p;
        int e = fac[f].exp;
        size_t old_sz = cur_sz;
        uint64_t mult = 1;
        for (int k = 1; k <= e; ++k) {
            mult *= p;
            for (size_t i = 0; i < old_sz; ++i) {
                divs[cur_sz++] = (seq_t)((uint64_t)divs[i] * mult);
            }
        }
    }

    /* ---------- find largest divisor < n ---------- */
    seq_t limit = (seq_t)n;
    seq_t best  = (seq_t)1;   /* 1 is always a divisor */

    for (size_t i = 0; i < cur_sz; ++i) {
        seq_t d = divs[i];
        if (d < limit && d > best)
            best = d;
    }

    return best;
}
