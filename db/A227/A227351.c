/*
 * A227351  Permutation of nonnegative integers: map each number by lengths of
 *          runs of zeros in its Zeckendorf expansion shifted once left to the
 *          number which has the same lengths of runs (in the same order, but
 *          alternatively of runs of 0's and 1's) in its binary
 *          representation.
 * https://oeis.org/A227351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 2583, u32 n <= 9949, u64 n <= 9949,
 *            u128 n <= 9949, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A227351:
   Convert n to its Zeckendorf representation, append one extra zero,
   extract the lengths of consecutive zero runs, and reconstruct a new
   binary number that alternates runs of 1s and 0s with the same lengths.
   This gives the permutation described in the OEIS entry. */

seq_t a(idx_t n)
{
    if (!n)
        return (seq_t)0;

    /* --- Fibonacci array up to n ---------------------------------- */
    /* We need at most about 32 Fibonacci numbers for n up to 64‑bit */
    idx_t fib[32];
    int sz = 2;
    fib[0] = (idx_t)1;
    fib[1] = (idx_t)2;
    while (fib[sz-1] <= n && sz < 31) {
        fib[sz] = fib[sz-1] + fib[sz-2];
        ++sz;
    }

    /* --- Zeckendorf bits ---------------------------------------- */
    unsigned char bits[32] = {0};
    idx_t rem = n;
    int i;
    for (i = sz-1; i >= 0 && rem > 0; --i) {
        if (fib[i] <= rem) {
            bits[i] = 1;
            rem -= fib[i];
            if (i > 0)           /* skip next fib to avoid adjacency */
                --i;
        }
    }

    /* --- Highest set bit --------------------------------------- */
    int high = -1;
    for (i = sz-1; i >= 0; --i) {
        if (bits[i]) { high = i; break; }
    }
    if (high < 0)                /* should not happen for n>0 */
        return (seq_t)0;

    /* --- Zero run lengths ------------------------------------- */
    idx_t zr[32];
    int zr_cnt = 0;
    int started = 0;
    idx_t zlen = 0;
    for (i = high; i >= 0; --i) {
        if (bits[i]) {
            if (started) {
                if (zlen)
                    zr[zr_cnt++] = zlen;
                zlen = 0;
            } else
                started = 1;
        } else if (started) {
            ++zlen;
        }
    }
    /* Append the final zero that was added after shifting */
    ++zlen;
    if (started && zlen)
        zr[zr_cnt++] = zlen;

    /* --- Build the result ------------------------------------- */
    seq_t res = (seq_t)0;
    for (int j = 0; j < zr_cnt; ++j) {
        idx_t len = zr[j];
        if ((j & 1) == 0) {
            /* run of 1s */
            res = (res << (unsigned)len) | (( ((seq_t)1 << (unsigned)len) - (seq_t)1 ));
        } else {
            /* run of 0s */
            res <<= (unsigned)len;
        }
    }
    return res;
}
