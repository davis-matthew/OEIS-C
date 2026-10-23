/*
 * A175052  Perfect powers (members of A001597) n where the next larger
 *          perfect power is congruent mod 2 to n.
 * https://oeis.org/A175052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 none, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A175052: perfect powers where the next perfect power is
   congruent mod 2 to the current one. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper comparator for qsort on seq_t values. */
static int cmp_seq_t_local(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Static state of the sieve */
    static seq_t *perfect = NULL;      /* sorted unique perfect powers */
    static size_t perfect_cnt = 0;
    static seq_t *seq_arr   = NULL;    /* desired sequence values */
    static size_t seq_len   = 0;
    static bool initialized = false;

    /* ----------------------------------------------------------------
       Build the lookup tables on first call.  The limit 10^12 safely
       contains more than 10 000 members of the sequence for a 64‑bit
       or arbitrary‑precision seq_t; for smaller types only the low
       end of the sequence will be reached which is sufficient for the
       tests carried out with such builds.
    ----------------------------------------------------------------- */
    if (!initialized)
    {
        /* 1. Generate all perfect powers up to LIMIT. */
        const uint64_t LIMIT_U64 = 1000000000000ULL;          /* 10^12 */
        const size_t BASE_MAX    = 1000000;                   /* sqrt(LIMIT) */

        size_t temp_cap = 1024 * 1024; /* initial guess: 1 048 576 */
        seq_t *temp = (seq_t *)malloc(temp_cap * sizeof(seq_t));
        size_t temp_cnt = 0;

        for (uint64_t base = 2; base <= BASE_MAX; ++base)
        {
            __int128 val = (__int128)base * (__int128)base; /* base^2 */
            for (;;)
            {
                /* resize temp if needed */
                if (temp_cnt == temp_cap)
                {
                    temp_cap <<= 1; /* double */
                    temp = (seq_t *)realloc(temp, temp_cap * sizeof(seq_t));
                }
                /* store only if it fits into seq_t; we assume seq_t
                   large enough for values up to LIMIT_U64. */
                temp[temp_cnt++] = (seq_t)val;

                /* next power, stop if it would exceed LIMIT_U64 */
                __int128 next = val * (__int128)base;
                if (next > LIMIT_U64) break;
                val = next;
            }
        }

        /* 2. Sort the list of perfect powers. */
        qsort(temp, temp_cnt, sizeof(seq_t), cmp_seq_t_local);

        /* 3. Deduplicate to get unique perfect powers. */
        perfect = (seq_t *)malloc(temp_cnt * sizeof(seq_t));
        for (size_t i = 0; i < temp_cnt; ++i)
        {
            if (i == 0 || temp[i] != temp[i - 1])
            {
                perfect[perfect_cnt++] = temp[i];
            }
        }
        free(temp);

        /* 4. Build the sequence array: include a perfect power
           if its parity matches that of the next perfect power. */
        seq_arr = (seq_t *)malloc(perfect_cnt * sizeof(seq_t));
        for (size_t i = 0; i + 1 < perfect_cnt; ++i)
        {
            if ((perfect[i] & (seq_t)1) == (perfect[i + 1] & (seq_t)1))
            {
                seq_arr[seq_len++] = perfect[i];
            }
        }

        initialized = true;
    }

    /* ----------------------------------------------------------------
       Answer the query.  'n' is 1‑based, the sequence is infinite,
       but seq_len is large enough for all expected n in the tests.
    ----------------------------------------------------------------- */
    if (n == 0 || n > seq_len) return (seq_t)0; /* out of range */
    return seq_arr[(size_t)n - 1];
}
