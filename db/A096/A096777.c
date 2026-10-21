/*
 * A096777  a(n) = a(n-1) + Sum_{k=1..n-1}(a(k) mod 2), a(1) = 1.
 * https://oeis.org/A096777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 443, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence:  a(1) = 1
   a(n) = a(n-1) + (number of odd terms among a(1)..a(n-1))
   Implementation uses memoisation so calls can be in any order.
*/
#include <stdlib.h>

static seq_t *seq_arr = NULL;
static idx_t seq_len = 0;     /* number of computed terms */
static idx_t seq_cap = 0;     /* allocated capacity of seq_arr   */
static uint64_t odd_cnt = 0;  /* number of odd terms so far */

/* Compute the n-th term of A096777 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, just for safety */

    /* initialise on first call */
    if (seq_len == 0) {
        seq_cap = 1;
        seq_arr = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_arr[0] = (seq_t)1;            /* a(1) = 1 */
        seq_len = 1;
        odd_cnt = 1;                      /* a(1) is odd */
    }

    /* extend the array up to n if necessary */
    if (n > seq_len) {
        for (idx_t i = seq_len + 1; i <= n; ++i) {
            seq_t prev = seq_arr[seq_len - 1];
            seq_t val  = prev + (seq_t)odd_cnt;   /* recurrence */

            /* ensure enough capacity */
            if (seq_len >= seq_cap) {
                idx_t newcap = seq_cap ? seq_cap << 1 : 1;
                if (newcap < n) newcap = n;
                seq_arr = (seq_t*)realloc(seq_arr, newcap * sizeof(seq_t));
                seq_cap = newcap;
            }

            seq_arr[seq_len] = val;
            seq_len++;

            /* update odd count for next step */
            if ((val & (seq_t)1) != (seq_t)0) odd_cnt++;
        }
    }

    return seq_arr[n - 1];
}
