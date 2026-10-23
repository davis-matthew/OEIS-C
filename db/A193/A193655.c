/*
 * A193655  Q-residue of the triangle p(n,k)=floor(1/2+(n+1)/(n+k+2)/2),
 *          0<=k<=n, where Q is the triangular array (t(i,j)) given by
 *          t(i,j)=1. (See Comments.)
 * https://oeis.org/A193655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) – 3*a(n-2) – 21*a(n-3) + 36*a(n-4) – 32*a(n-6) +
 *            16*a(n-7) for n >= 7, with initial values a(0)=1, a(1)=7,
 *            a(2)=29, a(3)=94, a(4)=280, a(5)=765, a(6)=2023
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define MAX_COMPUTED 20000   /* safe upper bound for the tests */

static seq_t cache[MAX_COMPUTED + 1];
static bool initialized = false;
static idx_t last_computed = 0;

/* Return the n-th term of sequence A193655. */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* Initialise the first seven terms. */
        cache[0] = 1;
        cache[1] = 7;
        cache[2] = 29;
        cache[3] = 94;
        cache[4] = 280;
        cache[5] = 765;
        cache[6] = 2023;
        last_computed = 6;
        initialized = true;
    }

    if (n <= last_computed) {
        return cache[n];
    }

    if (n > MAX_COMPUTED) {
        /* Compute without caching beyond MAX_COMPUTED. */
        seq_t a_nm7 = 0, a_nm6 = 0, a_nm5 = 0, a_nm4 = 0, a_nm3 = 0, a_nm2 = 0, a_nm1 = 0;
        /* Initialize with the known last values. */
        if (last_computed == 6) {
            a_nm1 = cache[6];
            a_nm2 = cache[5];
            a_nm3 = cache[4];
            a_nm4 = cache[3];
            a_nm5 = cache[2];
            a_nm6 = cache[1];
            a_nm7 = cache[0];
        } else {
            /* In this unlikely branch, compute up to last_computed first. */
            for (idx_t i = last_computed + 1; i <= n; ++i) {
                seq_t val = 5 * cache[i-1];
                val -= 3 * cache[i-2];
                val -= 21 * cache[i-3];
                val += 36 * cache[i-4];
                if (i >= 6) val -= 32 * cache[i-6];
                if (i >= 7) val += 16 * cache[i-7];
                cache[i] = val;
            }
            return cache[n];
        }

        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t val = 5 * a_nm1;
            val -= 3 * a_nm2;
            val -= 21 * a_nm3;
            val += 36 * a_nm4;
            if (i >= 6) val -= 32 * a_nm6;
            if (i >= 7) val += 16 * a_nm7;
            /* shift history forward */
            a_nm7 = a_nm6;
            a_nm6 = a_nm5;
            a_nm5 = a_nm4;
            a_nm4 = a_nm3;
            a_nm3 = a_nm2;
            a_nm2 = a_nm1;
            a_nm1 = val;
        }

        return a_nm1;
    }

    /* Compute and cache terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = 5 * cache[i-1];
        val -= 3 * cache[i-2];
        val -= 21 * cache[i-3];
        val += 36 * cache[i-4];
        if (i >= 6) val -= 32 * cache[i-6];
        if (i >= 7) val += 16 * cache[i-7];
        cache[i] = val;
    }

    last_computed = n;
    return cache[n];
}
