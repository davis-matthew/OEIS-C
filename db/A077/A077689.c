/*
 * A077689  Rearrangement of squares such that the most significant digits
 *          follow the cyclic pattern
 *          1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,...
 * https://oeis.org/A077689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A077689 –  
   Rearrangement of squares such that the most significant digits
   follow the cyclic pattern 1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,... */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *sqs[10];      /* sqs[d] holds squares with leading digit d (1..9)   */
static size_t len[10];      /* current number of stored squares per digit      */
static size_t cap[10];      /* allocated capacity for each vector             */
static idx_t  next_root = 1;/* next integer whose square has not yet been processed */

static inline int leading_digit(seq_t x)
{
    const seq_t ten = 10;
    while (x >= ten)
        x = x / ten;
    return (int)x;
}

/* Ensure that sqs[d] contains at least 'need' squares. New squares are generated in
   ascending order of the root. */
static void ensure_digit_vector(int d, size_t need)
{
    while (len[d] < need) {
        seq_t r   = (seq_t)next_root;
        seq_t sq  = r * r;                     /* square of r */
        int   ld  = leading_digit(sq);

        if (1 <= ld && ld <= 9) {
            if (len[ld] == cap[ld]) {
                size_t new_cap = cap[ld] ? cap[ld] * 2 : 16;
                seq_t *new_ptr = (seq_t *)realloc(sqs[ld], new_cap * sizeof(seq_t));
                if (!new_ptr) exit(1);
                sqs[ld] = new_ptr;
                cap[ld] = new_cap;
            }
            sqs[ld][len[ld]++] = sq;
        }
        next_root++;
    }
}

seq_t a(idx_t n)
{
    /* on first call initialise the per‑digit vectors */
    static bool initialized = false;
    if (!initialized) {
        for (int i = 1; i <= 9; ++i) {
            sqs[i] = NULL;
            len[i] = 0;
            cap[i] = 0;
        }
        initialized = true;
    }

    int  d = (int)((n - 1) % 9 + 1);                /* leading digit needed */
    size_t k = (size_t)((n - 1) / 9 + 1);            /* position within that digit */

    ensure_digit_vector(d, k);

    return sqs[d][k - 1];
}
