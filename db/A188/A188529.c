/*
 * A188529  Numbers which contain only the digit 3 in their base-4
 *          representation, with at most one exception. If the exception is
 *          the most-significant digit, it must be the digit 1 or 2, otherwise
 *          the exception must be the digit 2.
 * https://oeis.org/A188529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A188529: a(n) = the nth smallest positive integer whose base‑4
 * representation contains only the digit 3, with at most one exception.
 * If the exception is the most‑significant digit it may be 1 or 2;
 * otherwise the single exception must be the digit 2.  This function
 * enumerates the numbers length‑by‑length (L = 1,2,3…) and selects the
 * appropriate value for the requested index.  The computation uses only
 * integer arithmetic compatible with C11 and C++17. */

#include <stdint.h>  /* for UINT64_MAX */

static seq_t power4(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= 4;
    return r;
}

seq_t a(idx_t n)
{
    /* Find the base‑4 length L that contains the term n. */
    idx_t remaining = n;
    idx_t L = 1;
    while (1) {
        idx_t count = (L == 1) ? 3 : L + 2;   /* numbers of length L */
        if (remaining <= count)
            break;
        remaining -= count;
        ++L;
    }

    idx_t offset = remaining - 1;  /* zero‑based index within this length */

    /* For most lengths the powers of 4 fit in 64‑bits; only L=32
       reaches the 2^64 boundary where 4^32 would overflow.  We handle
       that case explicitly. */

    if (L <= 31) {
        seq_t pow4_Lminus1 = power4(L - 1);     /* 4^(L‑1)          */
        seq_t leading1 = pow4_Lminus1 * 2 - 1;  /* 2*4^(L‑1)‑1      */
        seq_t leading2 = pow4_Lminus1 * 3 - 1;  /* 3*4^(L‑1)‑1      */
        seq_t all3     = pow4_Lminus1 * 4 - 1;  /* 4^L‑1            */

        if (offset == 0)
            return leading1;
        if (offset == 1)
            return leading2;
        if (offset <= L) {                       /* internal exceptions */
            idx_t pos = L - offset;              /* 0‑based from LSB */
            seq_t pow4_pos = power4(pos);
            return all3 - pow4_pos;
        }
        return all3;                            /* all‑3 number     */
    }
    else if (L == 32) {
        seq_t pow4_Lminus1 = power4(31);        /* 4^31 = 2^62      */
        seq_t leading1 = pow4_Lminus1 * 2 - 1;  /* (1<<63) – 1      */
        seq_t leading2 = pow4_Lminus1 * 3 - 1;  /* 3*2^62 – 1       */
        seq_t all3     = (seq_t)UINT64_MAX;     /* 2^64 – 1         */

        if (offset == 0)
            return leading1;
        if (offset == 1)
            return leading2;
        if (offset <= 32) {
            idx_t pos = 32 - offset;            /* 0‑based from LSB */
            seq_t pow4_pos = 1;
            for (idx_t i = 0; i < pos; ++i)
                pow4_pos *= 4;
            return all3 - pow4_pos;
        }
        return all3;
    }
    else { /* L > 32: only possible with a larger integer type */
        seq_t pow4_Lminus1 = power4(L - 1);
        seq_t leading1 = pow4_Lminus1 * 2 - 1;
        seq_t leading2 = pow4_Lminus1 * 3 - 1;
        seq_t all3     = pow4_Lminus1 * 4 - 1;

        if (offset == 0)
            return leading1;
        if (offset == 1)
            return leading2;
        if (offset <= L) {
            idx_t pos = L - offset;
            seq_t pow4_pos = power4(pos);
            return all3 - pow4_pos;
        }
        return all3;
    }
}
