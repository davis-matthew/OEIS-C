/*
 * A099148  Take a <= b such that f(a)+f(b)=concatenation of a and b, where
 *          f(k)=k(k+3)/2 (A000096). Sequence gives values of a.
 * https://oeis.org/A099148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer a for which there exists a positive
 *            integer b such that f(a)+f(b) = concat(a,b) with f(k)=k(k+3)/2.
 *            The algorithm enumerates a upwards, and for each a checks all
 *            possible digit lengths d of b. For a given d the necessary
 *            condition b(b+1) = 2*a*10^d - a(a+3) is tested by s...
 * Range:     u8 n <= 16, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square root for unsigned __int128 */
static unsigned __int128 isqrt128(unsigned __int128 v)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64) - 1;
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* pre‑computed powers of ten up to 10^39 (fits in 128‑bit) */
static const unsigned __int128 pow10[40] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL,
    100000000000000000000ULL,
    1000000000000000000000ULL,
    10000000000000000000000ULL,
    100000000000000000000000ULL,
    1000000000000000000000000ULL,
    10000000000000000000000000ULL,
    100000000000000000000000000ULL,
    1000000000000000000000000000ULL,
    10000000000000000000000000000ULL,
    100000000000000000000000000000ULL,
    1000000000000000000000000000000ULL,
    10000000000000000000000000000000ULL,
    100000000000000000000000000000000ULL,
    1000000000000000000000000000000000ULL,
    10000000000000000000000000000000000ULL,
    100000000000000000000000000000000000ULL,
    1000000000000000000000000000000000000ULL,
    10000000000000000000000000000000000000ULL
};

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static idx_t next_a = 2;          /* first possible a */

    if (terms_cap == 0) {             /* initial allocation */
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    while (terms_len < n) {
        seq_t a_val = (seq_t)next_a++;
        unsigned __int128 a128 = (unsigned __int128)a_val;
        unsigned __int128 a_term = a128 * (a128 + 3);          /* a*(a+3) */

        bool found = false;
        /* try all possible digit lengths of b up to 38 digits */
        for (int d = 1; d < 38 && !found; ++d) {
            unsigned __int128 ten_d = pow10[d];
            unsigned __int128 R = 2 * a128 * ten_d - a_term;
            if (R == 0) continue;                 /* would give b=0 */
            if (R < 0) continue;                   /* skip negative R */

            unsigned __int128 t = 1 + 4 * R;        /* discriminant */
            unsigned __int128 sq = isqrt128(t);
            if (sq * sq != t) continue;            /* not a perfect square */
            if ((sq & 1ULL) == 0) continue;        /* discriminant even -> b non‑integer */

            unsigned __int128 b128 = (sq - 1) >> 1;
            seq_t b_val = (seq_t)b128;

            /* check that b has exactly d digits */
            if (b128 < pow10[d - 1] || b128 >= pow10[d]) continue;

            found = true;
        }

        if (found) {
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = a_val;
        }
    }

    return terms[n - 1];
}
