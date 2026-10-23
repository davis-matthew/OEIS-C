/*
 * A175414  Those positive integers that when written in binary, the lengths
 *          of the runs of 1 are distinct and the lengths of the runs of 0's
 *          are distinct, and at least one run of 0's is the same length as a
 *          run of 1's.
 * https://oeis.org/A175414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A175414 :   a(n) = the n-th positive integer whose binary
   representation has distinct run‑lengths of 1’s and distinct run‑lengths of 0’s,
   and at least one run‑length is common to both sets.
   The function enumerates these numbers in increasing order until it
   reaches the n‑th one.
*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/bitlen_seq_t.h"

static bool qualifies(seq_t x)
{
    if (x == (seq_t)0) return false;     /* skip zero */

    /* number of bits in x */
    unsigned int bitlen = bitlen_seq_t(x);
    size_t cnt1 = 0, cnt0 = 0;

    /* Temporary storage for run lengths */
    if (bitlen <= 512) {
        uint32_t runs1[512];
        uint32_t runs0[512];
        seq_t temp = x;

        while (temp > (seq_t)0) {
            int bit = ((temp & (seq_t)1) != 0) ? 1 : 0;
            uint32_t len = 0;
            while ((temp > (seq_t)0) && (((temp & (seq_t)1) != 0) == bit)) {
                ++len;
                temp >>= 1;
            }
            if (bit) {
                runs1[cnt1++] = len;
            } else {
                runs0[cnt0++] = len;
            }
        }

        if (cnt0 == 0) return false;       /* no zero runs */

        for (size_t i = 0; i < cnt1; ++i)
            for (size_t j = 0; j < i; ++j)
                if (runs1[i] == runs1[j]) return false;

        for (size_t i = 0; i < cnt0; ++i)
            for (size_t j = 0; j < i; ++j)
                if (runs0[i] == runs0[j]) return false;

        for (size_t i = 0; i < cnt1; ++i)
            for (size_t j = 0; j < cnt0; ++j)
                if (runs1[i] == runs0[j]) return true;

        return false;
    } else {
        /*
         * For very long numbers allocate dynamic arrays.
         * The number of runs cannot exceed bitlen, so this is safe.
         */
        uint32_t *runs1 = (uint32_t*)malloc(bitlen * sizeof(uint32_t));
        uint32_t *runs0 = (uint32_t*)malloc(bitlen * sizeof(uint32_t));
        if (!runs1 || !runs0) {
            free(runs1); free(runs0);
            return false;
        }

        seq_t temp = x;
        while (temp > (seq_t)0) {
            int bit = ((temp & (seq_t)1) != 0) ? 1 : 0;
            uint32_t len = 0;
            while ((temp > (seq_t)0) && (((temp & (seq_t)1) != 0) == bit)) {
                ++len;
                temp >>= 1;
            }
            if (bit) {
                runs1[cnt1++] = len;
            } else {
                runs0[cnt0++] = len;
            }
        }

        if (cnt0 == 0) {
            free(runs1); free(runs0);
            return false;
        }

        for (size_t i = 0; i < cnt1; ++i)
            for (size_t j = 0; j < i; ++j)
                if (runs1[i] == runs1[j]) {
                    free(runs1); free(runs0);
                    return false;
                }

        for (size_t i = 0; i < cnt0; ++i)
            for (size_t j = 0; j < i; ++j)
                if (runs0[i] == runs0[j]) {
                    free(runs1); free(runs0);
                    return false;
                }

        for (size_t i = 0; i < cnt1; ++i) {
            for (size_t j = 0; j < cnt0; ++j) {
                if (runs1[i] == runs0[j]) {
                    free(runs1); free(runs0);
                    return true;
                }
            }
        }

        free(runs1); free(runs0);
        return false;
    }
}

seq_t a(idx_t n)
{
    idx_t count = 0;        /* found terms so far */
    seq_t cur   = (seq_t)2; /* start from the smallest candidate */

    while (true) {
        if (qualifies(cur)) {
            ++count;
            if (count == n) return cur;
        }
        ++cur;
    }
}
