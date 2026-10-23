/*
 * A104205  If mod[n,4]=0 then a(n)=a(n-1), if mod[n,4]=1 then
 *          a(n)=a(n-2)+a(n-3), if mod[n,4]=2 then a(n)=a(n-3)+a(n-4)+a(n-5),
 *          if mod[n,4]=2 then a(n)=a(n-4)+a(n-5)+a(n-6)+a[n-7].
 * https://oeis.org/A104205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 41, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for sequence A104205:
   a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5, a(6)=8
   For n ≥ 7:
     if n mod 4 == 0: a(n) = a(n-1)
     if n mod 4 == 1: a(n) = a(n-2)+a(n-3)
     if n mod 4 == 2: a(n) = a(n-3)+a(n-4)+a(n-5)
     if n mod 4 == 3: a(n) = a(n-4)+a(n-5)+a(n-6)+a(n-7)
*/

#include <stdlib.h>
#include <stdint.h>

/* Static storage for memoized terms */
static seq_t *memo = NULL;
static idx_t memo_cap = 0;   /* allocated capacity */
static idx_t memo_size = 0;  /* number of terms stored (>= memo_size) */

/* Grow the memo array to at least 'need' elements */
static void ensure_capacity(idx_t need)
{
    if (need < memo_cap) return;
    idx_t new_cap = memo_cap ? memo_cap << 1 : 8;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_mem) exit(1);        /* allocation failure */
    memo = new_mem;
    memo_cap = new_cap;
}

/* Initial seeds for n=0..6 */
static void init_seeds(void)
{
    static const seq_t seeds[7] = { (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)2,
                                   (seq_t)3, (seq_t)5, (seq_t)8 };
    if (!memo) {
        memo_cap = 8;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) exit(1);
    }
    for (idx_t i = 0; i < 7; ++i) memo[i] = seeds[i];
    memo_size = 7;
}

seq_t a(idx_t n)
{
    /* Initialize on first call */
    if (!memo) init_seeds();

    /* Return seed values without recomputation */
    if (n < 7) return memo[n];

    /* Compute missing terms up to n */
    if (memo_size <= n) {
        ensure_capacity(n);
        for (idx_t i = memo_size; i <= n; ++i) {
            idx_t mod = i & 3U;   /* i % 4 for unsigned small values */
            switch (mod) {
                case 0: memo[i] = memo[i-1]; break;
                case 1: memo[i] = memo[i-2] + memo[i-3]; break;
                case 2: memo[i] = memo[i-3] + memo[i-4] + memo[i-5]; break;
                case 3: memo[i] = memo[i-4] + memo[i-5] + memo[i-6] + memo[i-7]; break;
            }
        }
        memo_size = n + 1;
    }
    return memo[n];
}
