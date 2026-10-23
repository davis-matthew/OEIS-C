/*
 * A045248  Numbers whose base-5 representation contains no 1's and exactly
 *          one 4.
 * https://oeis.org/A045248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A045248 – Base‑5 numbers with no digit 1 and exactly one digit 4.
    The function enumerates all such numbers in increasing order and
    returns the n‑th one (1‑based).  The algorithm counts how many
    admissible numbers exist for each possible prefix, so that it can
    skip whole blocks without explicit enumeration. */

#include <stdbool.h>

static seq_t pow3_cache[64];   /* pow3_cache[e] = 3^e */
static unsigned pow3_cache_used = 0;

/* Initialise and extend the power‑of‑3 cache up to exponent e. */
static void ensure_pow3(unsigned e)
{
    if (pow3_cache_used > e)          /* already prepared */
        return;
    if (pow3_cache_used == 0) {       /* first entry */
        pow3_cache[0] = (seq_t)1;
        pow3_cache_used = 1;
    }
    while (pow3_cache_used <= e) {
        pow3_cache[pow3_cache_used] =
            pow3_cache[pow3_cache_used - 1] * (seq_t)3;
        ++pow3_cache_used;
    }
}
static inline seq_t pow3_val(unsigned e)
{
    ensure_pow3(e);
    return pow3_cache[e];
}

/* Return the n‑th term of OEIS A045248 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for 0, but safe to return 0 */
        return (seq_t)0;

    /* -------- 1. determine the length L of the n‑th number -------- */
    seq_t cum = 0;          /* cumulative count of numbers of shorter length */
    unsigned L = 1;         /* current length hypothesis */

    while (1) {
        /* count(L) = 3^(L-1)  +  (L-1)*2*3^(L-2)   (for L ≥ 1) */
        seq_t countL = pow3_val(L - 1);
        if (L > 1)
            countL += (seq_t)(L - 1) * (seq_t)2 * pow3_val(L - 2);

        if (cum + countL >= (seq_t)n)
            break;          /* the required number has length L */

        cum += countL;
        ++L;
    }

    /* offset of the n‑th number inside the block of numbers of length L */
    seq_t r = (seq_t)n - cum - 1;   /* 0‑based */

    /* -------- 2. construct the number digit by digit -------- */
    seq_t  value = 0;
    bool used4 = false;            /* have we placed the single 4 yet? */

    for (unsigned pos = 0; pos < L; ++pos) {
        unsigned right = L - pos - 1;   /* digits to the right of pos */

        for (int d = 0; d < 5; ++d) {
            if (d == 1)                 /* digit 1 is forbidden */
                continue;
            if (pos == 0 && d == 0)     /* leading zero is forbidden */
                continue;
            if (used4 && d == 4)        /* only one 4 allowed */
                continue;

            seq_t ways;
            if (used4) {
                /* 4 already used – any of the remaining 3 digits at each
                   position, so 3^right possibilities. */
                ways = pow3_val(right);
            } else {
                if (d == 4) {           /* place the 4 now */
                    ways = pow3_val(right);
                } else {                /* 4 still to be placed later */
                    if (right == 0)       /* no positions left – impossible */
                        ways = 0;
                    else
                        ways = (seq_t)right * pow3_val(right - 1);
                }
            }

            if (r >= ways) {            /* skip this block */
                r -= ways;
                continue;
            }

            /* choose this digit */
            value = value * (seq_t)5 + (seq_t)d;
            if (d == 4)
                used4 = true;
            break;                       /* go to next position */
        }
    }

    return value;
}
