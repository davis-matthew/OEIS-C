/*
 * A081991  Numbers in ascending order formed by using all the digits of the
 *          next n numbers.
 * https://oeis.org/A081991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 4529, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   next_perm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A081991
 *  Sequence constructed by blocks:
 *  Let block k contain the next k consecutive integers after those
 *  already used.  Gather all decimal digits from those integers,
 *  form every distinct permutation that does NOT start with 0, and
 *  output the permutations in ascending numerical order.
 *  a(1) is the first term produced, and a(n) is the n‑th of all
 *  produced numbers for n ≥ 1.  This code implements the direct
 *  generation described above.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_perm.h"

/*  seq_t  and  idx_t  are supplied by the build system.  */

static seq_t *seq = NULL;          /* stored terms (1‑based, array 0‑based) */
static size_t seq_len = 0;         /* number of stored terms */
static size_t seq_cap = 0;         /* allocated capacity */

/*  numbers for the next block  */
static idx_t block_size = 1;       /* size of next block */
static idx_t block_start = 1;      /* first integer of next block */

/* ensure that the sequence array has enough capacity for `needed` terms    */
static void ensure_seq_cap(size_t needed) {
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* append a value to the stored sequence                               */
static void push_seq(seq_t v) {
    ensure_seq_cap(seq_len + 1);
    seq[seq_len++] = v;
}

/* generate terms until the sequence has at least `need` entries          */
static void generate_up_to(idx_t need) {
    if (seq_len >= need) return;

    /* temporary array to hold all digits of the current block */
    int digits[128];          /* enough for any reasonable block size */
    int dlen;

    while (seq_len < need) {
        /* collect digits of block_start .. block_start+block_size-1 */
        dlen = 0;
        idx_t limit = block_start + block_size;   /* exclusive */
        for (idx_t num = block_start; num < limit; ++num) {
            idx_t x = num;
            int rev[20];
            int rlen = 0;
            do {
                rev[rlen++] = (int)(x % 10);
                x /= 10;
            } while (x);
            for (int k = rlen - 1; k >= 0; --k)
                digits[dlen++] = rev[k];
        }

        /* sort digits ascending */
        for (int i = 1; i < dlen; ++i) {
            int key = digits[i];
            int j = i - 1;
            while (j >= 0 && digits[j] > key) {
                digits[j + 1] = digits[j];
                --j;
            }
            digits[j + 1] = key;
        }

        /* iterate all permutations, skipping those with a leading 0   */
        do {
            if (digits[0] == 0) continue;   /* leading zero illegal */

            seq_t val = 0;
            for (int i = 0; i < dlen; ++i)
                val = val * 10 + (seq_t)digits[i];
            push_seq(val);
        } while (next_perm(digits, dlen));

        /* advance to the next block */
        block_start += block_size;
        ++block_size;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset 0 is undefined */
    generate_up_to(n);
    return seq[n - 1];
}
