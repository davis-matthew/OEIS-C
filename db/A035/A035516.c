/*
 * A035516  Triangular array formed from Zeckendorf expansion of integers:
 *          repeatedly subtract the largest Fibonacci number you can until
 *          nothing remains.
 * https://oeis.org/A035516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = the n‑th term of the flattened triangular array * of
 *            Zeckendorf expansions of all integers. For n = 0, a(0) = 0. *
 *            For n ≥ 1, a(n) is obtained by repeatedly subtracting the
 *            largest * Fibonacci number (with Fibonacci sequence 1,2,3,5,…)
 *            from the * corresponding integer until the remainder i...
 * Range:     u8 n <= 775, u16 n <= 4005, u32 n <= 4005, u64 n <= 4005,
 *            u128 n <= 4005, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


static seq_t fibs[256];
static size_t fib_len = 0;

/* Ensure fibs[] contains all Fibonacci numbers ≤ limit. */
static void ensure_fibs_upto (seq_t limit)
{
    if (fib_len == 0) {          /* initialise */
        fibs[0] = (seq_t)1;
        fibs[1] = (seq_t)2;
        fib_len = 2;
    }
    while (fibs[fib_len - 1] <= limit) {
        if (fib_len >= sizeof(fibs) / sizeof(fibs[0]))
            break;                /* safety for very large limits */
        fibs[fib_len] = fibs[fib_len - 1] + fibs[fib_len - 2];
        ++fib_len;
    }
}

/* Return the largest Fibonacci number ≤ limit.  limit ≥ 1. */
static seq_t largest_fib_leq (seq_t limit)
{
    ensure_fibs_upto(limit);
    size_t i = fib_len;
    while (1) {
        --i;
        if (fibs[i] <= limit) return fibs[i];
        if (i == 0) break;
    }
    return (seq_t)0;          /* never reached for limit ≥ 1 */
}

/* Count Zeckendorf summands of the integer |number|. */
static idx_t zeckendorf_term_count (idx_t number)
{
    seq_t num = (seq_t)number;
    idx_t cnt = 0;
    while (num > (seq_t)0) {
        seq_t f = largest_fib_leq(num);
        num -= f;
        ++cnt;
    }
    return cnt;
}

/* Return the pos‑th Zeckendorf summand (0‑based) of the integer |number|. */
static seq_t zeckendorf_term (idx_t number, idx_t pos)
{
    seq_t num = (seq_t)number;
    idx_t cnt = 0;
    while (num > (seq_t)0) {
        seq_t f = largest_fib_leq(num);
        if (cnt == pos) return f;
        num -= f;
        ++cnt;
    }
    return (seq_t)0;  /* should never happen */
}

seq_t a (idx_t n)
{
    if (n == 0) return (seq_t)0;   /* the single element of row 0 */

    idx_t idx = n - 1;        /* skip the initial 0 */
    idx_t r = 1;              /* current integer whose Zeckendorf expansion is examined */

    while (1) {
        idx_t termcnt = zeckendorf_term_count(r);
        if (idx < termcnt) {
            return zeckendorf_term(r, idx);
        }
        idx -= termcnt;
        ++r;
    }
}
