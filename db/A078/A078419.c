/*
 * A078419  Numbers n such that h(n) = 2 h(n-1) where h(n) is the length of
 *          the sequence {n, f(n), f(f(n)), ...., 1} in the Collatz (or 3x +
 *          1) problem. (The earliest "1" is meant.)
 * https://oeis.org/A078419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078419: Numbers n such that h(n)=2·h(n-1)
   where h(m) is the length of the Collatz sequence from m
   down to 1 (including both endpoints).  The function
   a(n) returns the nth such natural number (a(1)=2). */

#include <stdlib.h>
#include <stdint.h>

#define CACHE_MAX 10000000ULL          /* cache size for Collatz lengths */

static seq_t *LEN_CACHE = NULL;        /* memoised lengths for values ≤ CACHE_MAX */
static uint64_t CUR = 2;               /* candidate integer to test next         */

static seq_t *SEQ = NULL;              /* list of found sequence values           */
static idx_t SEQ_LEN = 0;              /* number of elements currently stored     */
static idx_t SEQ_CAP = 0;              /* capacity of the SEQ array               */

/* push a new element onto the SEQ array, resizing as needed */
static void seq_push(seq_t val)
{
    if (SEQ_LEN == SEQ_CAP) {
        idx_t new_cap = SEQ_CAP == 0 ? 8 : SEQ_CAP * 2;
        seq_t *new_arr = (seq_t *)realloc(SEQ, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        SEQ = new_arr;
        SEQ_CAP = new_cap;
    }
    SEQ[SEQ_LEN++] = val;
}

/* compute the Collatz sequence length for n (1‑based),
   using a cache for values ≤ CACHE_MAX. */
static seq_t collatz_len(uint64_t n)
{
    if (n == 1) return (seq_t)1;

    /* initialise cache on first use */
    if (!LEN_CACHE) {
        LEN_CACHE = (seq_t *)calloc(CACHE_MAX + 1, sizeof(seq_t));
        if (!LEN_CACHE) abort();
        LEN_CACHE[1] = (seq_t)1;
    }

    /* cached result stocks immediate retrieval */
    if (n <= CACHE_MAX && LEN_CACHE[n] != (seq_t)0) return LEN_CACHE[n];

    /* iterative evaluation with an explicit stack */
    uint64_t stk[10000];
    size_t sz = 0;
    uint64_t cur = n;

    while (1) {
        if (cur <= CACHE_MAX && LEN_CACHE[cur] != (seq_t)0) {
            break;                     /* reached a cached value */
        }
        if (cur == 1) {
            break;                     /* reached the end */
        }
        stk[sz++] = cur;
        if (cur & 1) {
            cur = (uint64_t)(((__uint128_t)3 * cur) + 1);
        } else {
            cur = cur >> 1;
        }
    }

    seq_t len = (cur == 1) ? (seq_t)1 : LEN_CACHE[cur];

    /* unwind the stack, filling in cached lengths where possible */
    for (size_t i = sz; i-- > 0; ) {
        len += (seq_t)1;
        uint64_t val = stk[i];
        if (val <= CACHE_MAX) LEN_CACHE[val] = len;
    }
    return len;
}

/* public function required by the evaluation harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but guard against 0 */

    while (SEQ_LEN < n) {
        uint64_t cur_val = CUR;
        seq_t h_cur   = collatz_len(cur_val);
        seq_t h_prev  = collatz_len(cur_val - 1);

        if (h_cur == h_prev * (seq_t)2) {
            seq_push((seq_t)cur_val);
        }
        CUR++;
    }
    return SEQ[n - 1];
}
