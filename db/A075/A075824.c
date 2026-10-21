/*
 * A075824  Odd numbers that cannot be expressed as 2^k - 3^m where k and m
 *          are integers.
 * https://oeis.org/A075824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n-th odd integer not representable as 2^k – 3^m
   (k,m ≥ 0).  The value is obtained by sieving all differences 2^k – 3^m
   up to a dynamically increased limit until the required count is reached.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static storage for the sieve result and the sequence values */
static seq_t *seq_vals  = NULL;    /* array of sequence terms */
static idx_t  seq_len   = 0;       /* number of terms stored */
static bool   initialized = false;/* true once init_seq has finished */

/* in-place allocation helper (avoids C++ "new") */
static void *xmalloc(size_t n)
{
    void *p = malloc(n);
    return p;   /* caller casts to the appropriate type */
}

/* Initialize the sieve and sequence values up to at least 'needed' terms.
   The limit is doubled until enough terms are found. */
static void init_seq(idx_t needed)
{
    if (initialized && seq_len >= needed)
        return;

    /* free previous data if any */
    if (seq_vals)
    {
        free(seq_vals);
        seq_vals = NULL;
    }
    seq_len = 0;

    idx_t limit = 100000;              /* starting limit (adjustable) */
    while (1)
    {
        /* allocate a boolean array for representable numbers up to 'limit' */
        size_t sieve_size = (size_t)(limit + 1);
        bool *rep = (bool *)xmalloc(sieve_size * sizeof(bool));
        if (!rep)
            return;                    /* allocation failure */

        for (size_t i = 0; i < sieve_size; ++i)
            rep[i] = false;

        /* pre‑compute powers of 2 and 3 in 64‑bit integers */
        uint64_t pow2[128], pow3[128];
        idx_t k = 0, m = 0;

        uint64_t val = 1;
        while (val <= limit + 1 && k < (idx_t)(sizeof(pow2)/sizeof(pow2[0])))
        {
            pow2[k++] = val;
            if (val > UINT64_MAX / 2U) break;   /* prevent overflow */
            val <<= 1;
        }

        val = 1;
        while (val <= limit + 1 && m < (idx_t)(sizeof(pow3)/sizeof(pow3[0])))
        {
            pow3[m++] = val;
            if (val > UINT64_MAX / 3U) break;
            val *= 3U;
        }

        /* mark representable odd differences */
        for (idx_t i = 0; i < k; ++i)
        {
            for (idx_t j = 0; j < m; ++j)
            {
                if (pow2[i] <= pow3[j])
                    continue;          /* non‑positive difference */
                uint64_t diff = pow2[i] - pow3[j];
                if (diff > limit)
                    continue;
                if ((diff & 1U) == 1U)
                    rep[(size_t)diff] = true;
            }
        }

        /* count odd numbers not in the set up to 'limit' */
        idx_t cnt = 0;
        for (idx_t odd = 1; odd <= limit; odd += 2)
            if (!rep[(size_t)odd])
                ++cnt;

        if (cnt >= needed)
        {
            /* store all required terms */
            seq_vals = (seq_t *)xmalloc(cnt * sizeof(seq_t));
            idx_t pos = 0;
            for (idx_t odd = 1; odd <= limit; odd += 2)
                if (!rep[(size_t)odd])
                    seq_vals[pos++] = (seq_t)odd;
            seq_len = cnt;
            initialized = true;
            free(rep);
            break;
        }

        free(rep);
        limit <<= 1;   /* double the search limit and retry */
    }
}

/* public function requested by the test harness */
seq_t a(idx_t n)
{
    init_seq(n);
    return seq_vals[n - 1];
}
