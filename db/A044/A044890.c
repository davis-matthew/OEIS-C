/*
 * A044890  Base-4 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 822, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_t_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A044890: Base‑4 run lengths alternate: even, odd, even,...
 * a(n) is the n‑th positive integer whose base‑4 representation
 * has runs of equal digits of alternating even/odd lengths beginning
 * with an even length run.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_t_realloc.h"

/* Helper functions for memory management of seq_t arrays */
#ifdef __cplusplus
static seq_t *alloc_seq_t_array(size_t n)
{
    return new seq_t[n];
}
static void free_seq_t_array(seq_t *p)
{
    delete[] p;
}

#else
static seq_t *alloc_seq_t_array(size_t n)
{
    return (seq_t *)malloc(n * sizeof(seq_t));
}
static void free_seq_t_array(seq_t *p)
{
    free(p);
}

#endif

/* Main sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static cache: generated terms are stored for future calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* how many terms are stored */
    static size_t cache_cap = 0;  /* capacity of cache array */
    static idx_t next_test = 1;   /* next integer to test for validity */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* find the next valid integer */
        bool found = false;
        while (!found)
        {
            /* convert next_test to base‑4 digits (least significant first) */
            uint8_t digits[64];
            idx_t tmp = next_test;
            int dlen = 0;
            if (tmp == 0)
                digits[dlen++] = 0;
            else
                while (tmp)
                {
                    digits[dlen++] = (uint8_t)(tmp % 4);
                    tmp /= 4;
                }

            /* check alternating parity of run lengths (even first) */
            int run_len = 1;
            int expected_parity = 0; /* 0 = even, 1 = odd */
            bool ok = true;
            for (int i = dlen - 2; i >= 0; --i)
            {
                if (digits[i] == digits[i + 1]) /* same digit continues run */
                    ++run_len;
                else
                {
                    /* run ends – check its parity */
                    if ((run_len & 1) != expected_parity)
                    {
                        ok = false;
                        break;
                    }
                    expected_parity ^= 1;   /* toggle parity for next run */
                    run_len = 1;
                }
            }
            /* final run check */
            if (ok && ((run_len & 1) != expected_parity))
                ok = false;

            if (ok)
                found = true;
            else
                ++next_test;
        }

        /* grow cache if necessary */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
            seq_t *tmp = seq_t_realloc(cache, cache_len, new_cap);
            if (!tmp) /* allocation failure – undefined behaviour allowed */
                return 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* store the found term */
        cache[cache_len++] = (seq_t)next_test;
        ++next_test;
    }

    return cache[(size_t)n - 1];
}
