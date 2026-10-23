/*
 * A107228  Numbers having no even terms in their Zeckendorf representation.
 * https://oeis.org/A107228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer whose Zeckendorf representation *
 *            uses only Fibonacci numbers whose indices are NOT multiples of 3
 *            * (i.e., no even Fibonacci numbers). The function generates the
 *            * sequence on demand
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Fibonacci numbers used for Zeckendorf representation.
 * fib[0] = F2 = 1, fib[1] = F3 = 2, fib[2] = F4 = 3, ...  */
static seq_t *fib = NULL;
static idx_t fib_cnt = 0, fib_cap = 0;

/* Ensure the fib array has space for at least 'needed' elements. */
static void
ensure_fib(idx_t needed)
{
    if (fib_cap >= needed) return;
    size_t new_cap = fib_cap ? fib_cap * 2 : 8;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(fib, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    fib = tmp;
    fib_cap = new_cap;
}

/* Initialise the first two Fibonacci numbers. */
static void
init_fib(void)
{
    if (fib_cnt) return;                  /* already initialised */
    ensure_fib(2);
    fib[0] = (seq_t)1;  /* F2 */
    fib[1] = (seq_t)2;  /* F3 */
    fib_cnt = 2;
}

/* Grow the fib array until fib[fib_cnt-1] >= limit, or until overflow. */
static void
grow_fib_up_to(seq_t limit)
{
    init_fib();
    while (fib_cnt < 2 || fib[fib_cnt-1] < limit) {
        seq_t a = fib[fib_cnt-2];
        seq_t b = fib[fib_cnt-1];
#if defined(__cplusplus)
        /* For big‑int types the addition is safe; otherwise check 64‑bit overflow. */
        seq_t next = a + b;
#else
        /* 64‑bit or smaller: bail out on overflow. */
        if (b > (~(seq_t)0) - a) break;
        seq_t next = a + b;
#endif
        ensure_fib(fib_cnt + 1);
        fib[fib_cnt++] = next;
    }
}

/* Return true if 'x' has a Zeckendorf representation that uses
 * only Fibonacci numbers whose indices are NOT multiples of 3. */
static bool
valid_no_even_idx(seq_t x)
{
    seq_t rem = x;
    while (rem > 0) {
        grow_fib_up_to(rem);
        /* Find the largest fib[i] <= rem using binary search. */
        idx_t lo = 0, hi = fib_cnt - 1, pos = 0;
        while (lo <= hi) {
            idx_t mid = (lo + hi) >> 1;
            if (fib[mid] == rem) { pos = mid; break; }
            if (fib[mid] < rem) { pos = mid; lo = mid + 1; }
            else hi = mid - 1;
        }
        /* The original Fibonacci index is pos + 2.
         * Reject if that index is a multiple of 3 (even Fibonacci). */
        if (((pos + 2) % 3) == 0) return false;
        rem -= fib[pos];
    }
    return true;
}

/* Storage for already computed terms. */
static seq_t *term = NULL;
static idx_t term_len = 0, term_cap = 0;
static void
ensure_terms(idx_t needed)
{
    if (term_cap >= needed) return;
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(term, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    term = tmp;
    term_cap = new_cap;
}

/* Next number to test when building the sequence. */
static seq_t next_candidate = 1;

/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range – harmless */
    if (term_len < n) {
        ensure_terms(n);
        while (term_len < n) {
            seq_t cur = next_candidate++;
            if (valid_no_even_idx(cur)) {
                term[term_len++] = cur;
            }
        }
    }
    return term[n-1];
}
