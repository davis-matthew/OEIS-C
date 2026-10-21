/*
 * A230299  Define a sequence b_s by b_s(1)=s, b_s(k+1)=b_s(k)+(sum of digits
 *          of b_s(k)); a(n) is the number of steps needed for b_n to reach a
 *          term in one of b_0, b_1, b_3 or b_9, or a(n) = -1 if b_n never
 *          joins one of these four sequences.
 * https://oeis.org/A230299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A230299:  a(n) = number of steps for b_n to reach a term in
   one of the base‑0, base‑1, base‑3 or base‑9 sequences.  A step is
   b(k+1) = b(k) + sum_of_digits(b(k)).  The function returns 0 for
   n=0,1,2,3,4,6,8,9,... as shown in the OEIS text.  The implementation
   builds the reference sequences b_1, b_3, b_9 on demand and checks for
   membership via binary search.  Since b_s are strictly increasing the
   search is safe. */
#include <stdlib.h>
#include <stdbool.h>

/* forward declarations of helpers */
static inline seq_t digit_sum(seq_t v);
static inline seq_t next_b(seq_t v);

/* arrays holding the b_1, b_3 and b_9 sequences (strictly increasing) */
static seq_t *b1 = NULL;  static size_t b1_sz = 0, b1_cap = 0;
static seq_t *b3 = NULL;  static size_t b3_sz = 0, b3_cap = 0;
static seq_t *b9 = NULL;  static size_t b9_sz = 0, b9_cap = 0;

/* ------------------------------------------------------------------ */
/* Helper: digit sum in base 10 */
static inline seq_t digit_sum(seq_t v)
{
    seq_t s = 0;
    while (v != 0) {
        s += v % 10;
        v /= 10;
    }
    return s;
}

/* Helper: next term of a b_s sequence */
static inline seq_t next_b(seq_t v)
{
    return v + digit_sum(v);
}

/* ------------------------------------------------------------------ */
/* Ensure the first argument sequence contains all terms ≤ limit.  The
   sequence starts with the initial seed (1, 3 or 9). */
static int ensure_b1(seq_t limit)
{
    if (b1_sz == 0) {
        b1_cap = 16;
        b1 = (seq_t *)malloc(b1_cap * sizeof(seq_t));
        if (!b1) return -1;
        b1[0] = 1;
        b1_sz = 1;
    }
    while (b1_sz > 0 && b1[b1_sz - 1] < limit) {
        seq_t next = next_b(b1[b1_sz - 1]);
        if (b1_sz == b1_cap) {
            size_t new_cap = b1_cap * 2;
            seq_t *tmp = (seq_t *)realloc(b1, new_cap * sizeof(seq_t));
            if (!tmp) return -1;
            b1 = tmp;
            b1_cap = new_cap;
        }
        b1[b1_sz++] = next;
    }
    return 0;
}

static int ensure_b3(seq_t limit)
{
    if (b3_sz == 0) {
        b3_cap = 16;
        b3 = (seq_t *)malloc(b3_cap * sizeof(seq_t));
        if (!b3) return -1;
        b3[0] = 3;
        b3_sz = 1;
    }
    while (b3_sz > 0 && b3[b3_sz - 1] < limit) {
        seq_t next = next_b(b3[b3_sz - 1]);
        if (b3_sz == b3_cap) {
            size_t new_cap = b3_cap * 2;
            seq_t *tmp = (seq_t *)realloc(b3, new_cap * sizeof(seq_t));
            if (!tmp) return -1;
            b3 = tmp;
            b3_cap = new_cap;
        }
        b3[b3_sz++] = next;
    }
    return 0;
}

static int ensure_b9(seq_t limit)
{
    if (b9_sz == 0) {
        b9_cap = 16;
        b9 = (seq_t *)malloc(b9_cap * sizeof(seq_t));
        if (!b9) return -1;
        b9[0] = 9;
        b9_sz = 1;
    }
    while (b9_sz > 0 && b9[b9_sz - 1] < limit) {
        seq_t next = next_b(b9[b9_sz - 1]);
        if (b9_sz == b9_cap) {
            size_t new_cap = b9_cap * 2;
            seq_t *tmp = (seq_t *)realloc(b9, new_cap * sizeof(seq_t));
            if (!tmp) return -1;
            b9 = tmp;
            b9_cap = new_cap;
        }
        b9[b9_sz++] = next;
    }
    return 0;
}

/* Binary search in an increasing array */
static inline bool contains(const seq_t *arr, size_t sz, seq_t x)
{
    size_t lo = 0, hi = sz;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] == x) return true;
        if (arr[mid] < x) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    idx_t  steps = 0;

    while (true) {
        if (cur == 0) return (seq_t)steps;               /* b_0 is all zeros */

        if (ensure_b1(cur) == 0 && contains(b1, b1_sz, cur))
            return (seq_t)steps;
        if (ensure_b3(cur) == 0 && contains(b3, b3_sz, cur))
            return (seq_t)steps;
        if (ensure_b9(cur) == 0 && contains(b9, b9_sz, cur))
            return (seq_t)steps;

        cur = next_b(cur);
        ++steps;
        /* under the conjecture the loop always terminates */
    }
}
