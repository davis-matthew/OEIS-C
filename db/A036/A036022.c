/*
 * A036022  Number of partitions of n into parts not of form 4k+2, 16k, 16k+5
 *          or 16k-5.
 * https://oeis.org/A036022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Sequence A036022 – number of partitions of n into * parts not of
 *            form 4k+2, 16k, 16k+5 or 16k-5. * * The generating function is
 *            the Euler transform of the period‑16 * sequence
 *            [1,0,1,1,0,0,1,1,1,0,0,1,1,0,1,0,...]. * Thus the allowed part
 *            sizes are those n with residue * 1,3,4,7,8,9,12,13,15 modu...
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"   /* for seq_alloc() and free() */
#include <stddef.h>

static seq_t *dp = NULL;     /* array holding computed partition counts */
static idx_t  dp_max = 0;    /* maximum n for which dp[0..dp_max] are valid */

/* residues modulo 16 that correspond to allowed part sizes */
static const unsigned char allowed_res[16] = {
    0, /* 0  not allowed */
    1, /* 1  allowed */
    0, /* 2  disallowed (4k+2) */
    1, /* 3  allowed */
    1, /* 4  allowed */
    0, /* 5  disallowed (16k+5) */
    0, /* 6  disallowed (4k+2) */
    1, /* 7  allowed */
    1, /* 8  allowed */
    1, /* 9  allowed */
    0, /*10  disallowed (4k+2) */
    0, /*11  disallowed (16k-5) */
    1, /*12  allowed */
    1, /*13  allowed */
    0, /*14  disallowed (4k+2) */
    1  /*15  allowed */
};

#define PRECOMP_LIMIT 20000   /* precompute up to this value on first call */

/* Computes a(n) */
seq_t a(idx_t n)
{
    /* Ensure DP array is allocated and precomputed up to at least n */
    if (dp == NULL || n > dp_max)
    {
        /* Determine the limit we need to compute up to */
        idx_t limit = (dp == NULL) ? (n > PRECOMP_LIMIT ? n : PRECOMP_LIMIT)
                                   : n;

        /* Allocate new array */
        seq_t *newdp = seq_alloc(limit + 1);

        /* Initialize */
        for (idx_t i = 0; i <= limit; ++i)
            newdp[i] = 0;
        newdp[0] = 1;

        /* Classic coin‑change DP over allowed part sizes */
        for (idx_t part = 1; part <= limit; ++part)
        {
            if (allowed_res[part & 15])
            {
                for (idx_t sum = part; sum <= limit; ++sum)
                {
                    newdp[sum] += newdp[sum - part];
                }
            }
        }

        /* Replace old DP array */
        if (dp != NULL)
            free(dp);
        dp = newdp;
        dp_max = limit;
    }

    /* If the requested n is beyond the current precomputed range,
       expand and recompute from scratch (this happens at most once
       per distinct larger n). */
    if (n > dp_max)
    {
        idx_t limit = n;
        seq_t *newdp = seq_alloc(limit + 1);

        for (idx_t i = 0; i <= limit; ++i)
            newdp[i] = 0;
        newdp[0] = 1;

        for (idx_t part = 1; part <= limit; ++part)
        {
            if (allowed_res[part & 15])
            {
                for (idx_t sum = part; sum <= limit; ++sum)
                {
                    newdp[sum] += newdp[sum - part];
                }
            }
        }

        if (dp != NULL)
            free(dp);
        dp = newdp;
        dp_max = limit;
    }

    return dp[n];
}
