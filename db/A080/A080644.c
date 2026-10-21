/*
 * A080644  a(1) = 5; for n>1, a(n) is taken to be the smallest integer
 *          greater than a(n-1) which is consistent with the condition "n is a
 *          member of the sequence if and only if a(n) is divisible by 6".
 * https://oeis.org/A080644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest integer > a(n-1) such that n∈{a(k)} ⇔ a(n) ≡ 0
 *            (mod 6)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static memoization table for computed values */
static seq_t *memo = NULL;
static idx_t memo_sz = 0;    /* allocated size */
static idx_t memo_cnt = 0;   /* number of terms stored (0 unused) */

/* ensure memo array has space for indices up to n (1‑based) */
static void ensure(idx_t n)
{
    if (n <= memo_cnt) return;
    /* allocate space for indices up to n (inclusive) */
    if (n + 1 > memo_sz) {
        idx_t new_sz = memo_sz ? memo_sz * 2 : 1;
        while (new_sz < n + 1) new_sz <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_sz * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed; abort */
            exit(1);
        }
        memo = tmp;
        memo_sz = new_sz;
    }
    /* initialise first missing term if still uninitialized */
    if (memo_cnt == 0) {
        memo[1] = (seq_t)5;
        memo_cnt = 1;
    }
    /* compute missing terms up to n */
    for (idx_t k = memo_cnt + 1; k <= n; ++k) {
        /* does k belong to the set of values already computed? */
        seq_t target = (seq_t)k;
        bool present = false;
        idx_t lo = 1, hi = memo_cnt;
        while (lo <= hi) {
            idx_t mid = lo + (hi - lo) / 2;
            if (memo[mid] == target) { present = true; break; }
            else if (memo[mid] < target) lo = mid + 1;
            else hi = mid - 1;
        }

        /* pick next value */
        seq_t prev = memo[k - 1];
        seq_t next;
        if (present) { /* need divisible by 6 */
            next = prev + 1;
            seq_t r = next % (seq_t)6;
            if (r != 0) next += (seq_t)(6 - r);
        } else {       /* need not divisible by 6 */
            next = prev + 1;
            if (next % (seq_t)6 == 0) next += 1;
        }
        memo[k] = next;
    }
    memo_cnt = n;
}

/* public function */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1 */
    ensure(n);
    return memo[n];
}
