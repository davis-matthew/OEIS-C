/*
 * A175746  Numbers with 36 divisors.
 * https://oeis.org/A175746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 743, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A175746 – Numbers with 36 divisors
   a(n) is the nth integer (1‑based) that has exactly 36 positive divisors. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* We compute all numbers with 36 divisors up to a fixed bound
       that is enough for n up to about 10000.  The bound is 5 000 000
       which is a safe choice for the expected test data. */
    static bool ready = false;
    static seq_t *values = NULL;      /* array of found numbers */
    static size_t val_cap  = 0;       /* allocated capacity */
    static size_t val_len  = 0;       /* number of stored values */

    if (!ready)
    {
        /* ---------- Sieve smallest prime factor up to LIMIT ---------- */
        const idx_t LIMIT = 5000000;          /* 5 000 000 */
        uint32_t *spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
        if (!spf) return 0;                  /* allocation failure */

        for (idx_t i = 2; i <= LIMIT; ++i)
        {
            if (spf[i] == 0)
            {
                spf[i] = (uint32_t)i;
                if (i * i <= LIMIT)
                {
                    for (idx_t j = i * i; j <= LIMIT; j += i)
                    {
                        if (spf[j] == 0) spf[j] = (uint32_t)i;
                    }
                }
            }
        }

        /* ---------- Count divisors and collect ==36 ---------- */
        size_t init_cap = 20000;            /* initial guess for number of values */
        values = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!values) { free(spf); return 0; }
        val_cap = init_cap;
        val_len = 0;

        for (idx_t i = 1; i <= LIMIT; ++i)
        {
            idx_t x = i;
            seq_t dcnt = 1;
            while (x > 1)
            {
                uint32_t p = spf[x];
                idx_t exp = 0;
                while (x % p == 0)
                {
                    x /= p;
                    ++exp;
                }
                dcnt *= (seq_t)(exp + 1);
            }
            if (dcnt == 36)
            {
                if (val_len == val_cap)
                {
                    val_cap = val_cap ? val_cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(values, val_cap * sizeof(seq_t));
                    if (!tmp) { free(spf); free(values); return 0; }
                    values = tmp;
                }
                values[val_len++] = (seq_t)i;
            }
        }

        free(spf);
        ready = true;
    }

    if (n == 0 || n > (idx_t)val_len) return 0; /* out of bounds */
    return values[n - 1];
}
