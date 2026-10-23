/*
 * A140100  Start with Y(0)=0, X(1)=1, Y(1)=2. For n > 1, choose least
 *          positive integers Y(n) > X(n) such that neither Y(n) nor X(n)
 *          appear in {Y(k), 1 <= k < n} or {X(k), 1 <= k < n} and such that
 *          Y(n) - X(n) does not appear in {Y(k) - X(k), 1 <= k < n} or {Y(k)
 *          + X(k), 1 <= k < n}; sequence gives X(n) (for...
 * https://oeis.org/A140100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 165, u16 n <= 9808, u32 n <= 9808, u64 n <= 9808,
 *            u128 n <= 9808, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A140100: X(n) part of the greedy complementary construction.
   The algorithm incrementally builds X and Y as described in the OEIS
   entry.  For each n it selects the smallest unused integer X(n+1)
   and the smallest unused integer Y> X such that the difference
   Y-X is not equal to any earlier difference or earlier sum.
   The arrays X, Y, used, diff_used and sum_used are kept in static
   memory and reused, allowing a(n) to be called in any order.
   The routine runs in O(n) time with modest memory for n up to
   at least 10 000.                                                       */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t current_n = 0;          /* number of pairs already computed */
static seq_t *X = NULL;              /* values X(1..current_n) */
static seq_t *Y = NULL;              /* values Y(1..current_n) */
static size_t xy_cap = 0;            /* capacity of X and Y arrays */

/* Flags to mark numbers that have already been used in X or Y.   */
static unsigned char *used = NULL;   /* used[i]==1 iff i is in X or Y */
static size_t used_cap = 0;

/* Flags to mark differences Y-X that have already appeared.     */
static unsigned char *diff_used = NULL;
static size_t diff_cap = 0;

/* Flags to mark sums Y+X that have already appeared.           */
static unsigned char *sum_used = NULL;
static size_t sum_cap = 0;

/* ------------------------------------------------------------------ */
/* Resize an unsigned char array so that index 'need' is valid.        */
static void ensure_arr(unsigned char **arr, size_t *cap, size_t need) {
    if (need < *cap) return;
    size_t newcap = (*cap == 0) ? 64 : *cap << 1;
    while (newcap <= need) newcap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(*arr, newcap);
    if (!tmp) abort();
    memset(tmp + *cap, 0, newcap - *cap);
    *arr = tmp;
    *cap = newcap;
}

/* Ensure arrays for used, diff, and sum are large enough for 'val'.  */
static void ensure_used_capacity(idx_t val) {
    if (val < 0) return;
    ensure_arr(&used,      &used_cap,      (size_t)val );
    ensure_arr(&diff_used, &diff_cap,      (size_t)(val + 10));
    /* sums can be up to ~2*val, but we enlarge lazily when they are
       accessed.                                               */
}

/* Ensure X and Y arrays have capacity for at least 'n' elements.    */
static void ensure_xy_cap(idx_t n) {
    if ((idx_t)xy_cap >= n) return;
    size_t newcap = (xy_cap == 0) ? 64 : xy_cap << 1;
    while (newcap < (size_t)n) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(X, newcap * sizeof(seq_t));
    if (!tmp) abort();   X = tmp;
    tmp = (seq_t *)realloc(Y, newcap * sizeof(seq_t));
    if (!tmp) abort();   Y = tmp;
    xy_cap = newcap;
}

/* Ensure sum_used array is large enough for index 'val'.           */
static void ensure_sum_capacity(idx_t val) {
    if ((size_t)val < sum_cap) return;
    size_t newcap = (sum_cap == 0) ? 128 : sum_cap << 1;
    while (newcap <= (size_t)val) newcap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(sum_used, newcap);
    if (!tmp) abort();
    memset(tmp + sum_cap, 0, newcap - sum_cap);
    sum_used = tmp;
    sum_cap  = newcap;
}

/* One–time initialisation: the first pair X(1)=1, Y(1)=2.           */
static void init_once(void) {
    ensure_xy_cap(1);
    X[0] = (seq_t)1;
    Y[0] = (seq_t)2;
    current_n = 1;

    ensure_used_capacity(2);
    used[1] = used[2] = 1;
    diff_used[1] = 1;          /* Y1 - X1 = 1 */
    ensure_sum_capacity(3);
    sum_used[3] = 1;           /* Y1 + X1 = 3 */
}

/* Build the sequences up to index n (inclusive).                    */
static void build_up_to(idx_t n) {
    if (current_n == 0) init_once();

    while (current_n < n) {
        /* Find the smallest unused integer X for the next pair.   */
        idx_t x = 1;
        for (;;) {
            ensure_used_capacity(x);
            if (!used[x]) break;
            ++x;
        }

        /* Search for the smallest Y > X satisfying the constraints. */
        idx_t y = x + 1;
        for (;;) {
            ensure_used_capacity(y);
            if (used[y]) { ++y; continue; }

            idx_t d = y - x;          /* difference  */
            idx_t s = y + x;          /* sum        */

            ensure_arr(&diff_used, &diff_cap, (size_t)d);
            ensure_sum_capacity(s);

            if (diff_used[d] || sum_used[d]) { ++y; continue; }

            /* Found suitable (X,Y).  Record and proceed.        */
            used[x] = used[y] = 1;
            diff_used[d] = 1;
            sum_used[s] = 1;

            ensure_xy_cap(current_n + 1);
            X[current_n] = (seq_t)x;
            Y[current_n] = (seq_t)y;
            ++current_n;
            break;
        }
    }
}

/* Exported function required by the OEIS harness.                    */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */
    build_up_to(n);
    return X[n - 1];
}
