/*
 * A074499  Sum of three perfect powers.
 * https://oeis.org/A074499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A074499 – Sum of three perfect powers
    The function a(n) returns the n‑th smallest integer that can be
    written as the sum of three perfect powers (a perfect power is a
    number of the form a^b, with a ≥ 1 and b ≥ 2; 1 counts as a
    perfect power).  The sequence is computed by enumerating all
    perfect powers up to a growing bound and marking every sum of
    three of them. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* comparison function for qsort on seq_t */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va > vb) - (va < vb);
}

seq_t a(idx_t n)
/* Return the n‑th term of A074499 */
{
    /* static data to store the sequence once computed */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    /* current upper bound for enumeration of perfect powers and sums */
    static seq_t limit = 20000;    /* start with a reasonable bound */

    /* the requested n must be >=1; if smaller, return 0 */
    if (n == 0) return 0;

    /* compute terms until we have at least n of them */
    while (seq_len < (size_t)n)
    {
        /* ---- generate all perfect powers <= limit ---- */
        size_t pow_cap = 16;
        seq_t *powers = (seq_t *)malloc(pow_cap * sizeof(seq_t));
        if (!powers) return 0;
        size_t pow_len = 0;

        /* include the perfect power 1 */
        powers[pow_len++] = (seq_t)1;

        for (seq_t base = 2; base * base <= limit; ++base)
        {
            seq_t val = base * base;             /* exponent 2 */
            while (val <= limit)
            {
                /* avoid inserting duplicate values (e.g. 64 = 8^2 = 4^3) */
                if (!(pow_len && powers[pow_len - 1] == val))
                {
                    if (pow_len == pow_cap)
                    {
                        pow_cap <<= 1;
                        seq_t *tmp = (seq_t *)realloc(powers, pow_cap * sizeof(seq_t));
                        if (!tmp) { free(powers); return 0; }
                        powers = tmp;
                    }
                    powers[pow_len++] = val;
                }

                if (val > limit / base) break;    /* next multiplication would overflow limit */
                val *= base;
            }
        }

        /* sort the list of powers so that later loops can break early */
        qsort(powers, pow_len, sizeof(seq_t), cmp_seq_t);

        /* ---- mark all sums of three perfect powers ---- */
        bool *marked = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
        if (!marked) { free(powers); return 0; }

        for (size_t i = 0; i < pow_len; ++i)
        {
            seq_t pi = powers[i];
            for (size_t j = i; j < pow_len; ++j)
            {
                seq_t pj = powers[j];
                seq_t sum2 = pi + pj;
                if (sum2 > limit) break;

                for (size_t k = j; k < pow_len; ++k)
                {
                    seq_t pk = powers[k];
                    seq_t s = sum2 + pk;
                    if (s > limit) break;
                    marked[(size_t)s] = true;
                }
            }
        }

        /* ---- append to the global sequence ---- */
        for (size_t v = 0; v <= (size_t)limit; ++v)
        {
            if (marked[v])
            {
                if (seq_len == seq_cap)
                {
                    seq_cap = seq_cap ? seq_cap << 1 : 16;
                    seq_t *tmp = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                    if (!tmp) { free(powers); free(marked); return 0; }
                    seq = tmp;
                }
                seq[seq_len++] = (seq_t)v;
            }
        }

        free(powers);
        free(marked);

        /* if still not enough terms, enlarge the bound and retry */
        if (seq_len < (size_t)n) limit <<= 1;
    }

    return seq[n - 1];
}
