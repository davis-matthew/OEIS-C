/*
 * A023499  a(n) = b(n) + d(n), where b(n) = ( (n+1)st Fibonacci number) and
 *          d(n) = (n-th number that is 1, 2, or 3, or is not a Lucas number).
 * https://oeis.org/A023499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F_{n+2} + d(n) * where F_k is the k-th Fibonacci number
 *            (F_0 = 0, F_1 = 1), * and d(n) is the n‑th positive integer that
 *            is either 1, 2, 3, or not a * Lucas number (Lucas numbers with
 *            index ≥ 4 are excluded)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- Fibonacci cache ---------- */
static seq_t *fib_cache = NULL;
static size_t fib_cap = 0;
static idx_t fib_len = 0;

static seq_t fib_val(idx_t n)
{
    if (!fib_cache) {
        fib_cap = 8;
        fib_cache = (seq_t*)malloc(fib_cap * sizeof(seq_t));
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
        fib_len = 2;
    }
    while (fib_len <= n) {
        if (fib_len == fib_cap) {
            fib_cap <<= 1;
            fib_cache = (seq_t*)realloc(fib_cache, fib_cap * sizeof(seq_t));
        }
        fib_cache[fib_len] = fib_cache[fib_len-1] + fib_cache[fib_len-2];
        ++fib_len;
    }
    return fib_cache[n];
}

/* ---------- Lucas cache ---------- */
static seq_t *lucas_cache = NULL;
static size_t lucas_cap = 0;
static size_t lucas_len = 0;
static seq_t l_prev2 = (seq_t)2;   /* L0 */
static seq_t l_prev1 = (seq_t)1;   /* L1 */

/* Ensure all Lucas numbers up to and including 'limit' are stored */
static void ensure_lucas_up_to(seq_t limit)
{
    while (true) {
        seq_t nxt = l_prev1 + l_prev2;
        if (nxt > limit)
            break;
        if (lucas_len == lucas_cap) {
            lucas_cap = (lucas_cap ? lucas_cap * 2 : 8);
            lucas_cache = (seq_t*)realloc(lucas_cache, lucas_cap * sizeof(seq_t));
        }
        lucas_cache[lucas_len++] = nxt;
        l_prev2 = l_prev1;
        l_prev1 = nxt;
    }
}

/* ---------- d(n) cache ---------- */
static seq_t *d_cache = NULL;
static size_t d_cap = 0;
static idx_t d_len = 0;
static seq_t next_candidate = (seq_t)1;

/* Compute the n-th value of d(n) */
static seq_t d_value(idx_t n)
{
    if (!d_cache) {
        d_cap = 16;
        d_cache = (seq_t*)malloc(d_cap * sizeof(seq_t));
        d_len = 0;
    }

    while (d_len <= n) {
        /* Determine if next_candidate should be included. */
        bool include = (next_candidate <= (seq_t)3);   /* 1, 2, 3 are always included */
        if (!include) {
            ensure_lucas_up_to(next_candidate);
            bool is_lucas = false;
            for (size_t i = 0; i < lucas_len; ++i) {
                if (lucas_cache[i] == next_candidate) {
                    is_lucas = true;
                    break;
                }
                if (lucas_cache[i] > next_candidate)
                    break;
            }
            if (!is_lucas)
                include = true;
        }

        if (include) {
            if (d_len == d_cap) {
                d_cap <<= 1;
                d_cache = (seq_t*)realloc(d_cache, d_cap * sizeof(seq_t));
            }
            d_cache[d_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return d_cache[n];
}

/* ---------- Public function as required ---------- */
seq_t a(idx_t n)
{
    seq_t fib_part = fib_val(n + (idx_t)2);  /* F_{n+2} */
    seq_t d_part   = d_value(n);            /* d(n)   */
    return fib_part + d_part;
}
