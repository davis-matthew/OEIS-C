/*
 * A062877  0 and numbers representable as a sum of distinct odd-indexed
 *          Fibonacci numbers.
 * https://oeis.org/A062877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 3071, u32 n <= 3071, u64 n <= 3071,
 *            u128 n <= 3071, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062877: numbers representable as a sum of distinct odd‑indexed
 * Fibonacci numbers.  The sequence is defined by
 *
 *     a(n) = Σ_{i ≥ 0} ( (n >> i) & 1 ) * F_{2i+1},
 *
 * where F_1 = 1, F_2 = 1, F_{k+1} = F_k + F_{k-1} is the standard Fibonacci
 * sequence and F_{2i+1} denotes the Fibonacci number with an odd index.
 *
 * For n = 0 the empty sum yields a(0) = 0.  The implementation below
 * pre‑computes the required odd‑indexed Fibonacci numbers on demand and
 * evaluates the sum using the binary representation of n.  It works for
 * any `seq_t` (built‑in integer or arbitrary‑precision) and any
 * `idx_t` (uintptr_t/unsigned long long).  No dynamic allocation is
 * required.
 */

#include <stddef.h>

/*---------------------------------------------------------------------*
 *  State for Fibonacci generation                                    *
 *---------------------------------------------------------------------*/
static seq_t  odd_fibs[128];   /* F_1, F_3, F_5, …  */
static size_t odd_len  = 0;    /* number of stored odd fibs              */
static seq_t  fib_prev;        /* F_{k-1} */
static seq_t  fib_curr;        /* F_k     */
static idx_t  fib_idx;         /* index of fib_curr (1‑based)            */

/*---------------------------------------------------------------------*
 *  Ensure that odd_fibs contains at least 0 … needed indices          *
 *---------------------------------------------------------------------*/
static void ensure_odd_fibs(size_t needed)
{
    /* Lazy one‑time initialization */
    if (odd_len == 0) {
        odd_fibs[0] = (seq_t)1;   /* F_1 */
        odd_len   = 1;
        fib_prev  = (seq_t)1;     /* F_1 */
        fib_curr  = (seq_t)1;     /* F_2 */
        fib_idx   = 2;            /* current index of fib_curr */
    }

    /* Generate further Fibonacci numbers until we have index 'needed' */
    while (odd_len <= needed) {
        seq_t next = fib_prev + fib_curr; /* F_{k+1} */
        fib_prev  = fib_curr;
        fib_curr  = next;
        fib_idx++;

        /* Store only odd‑indexed Fibonacci numbers */
        if ((fib_idx & 1) != 0) {
            odd_fibs[odd_len] = fib_curr;
            odd_len++;
        }
    }
}

/*---------------------------------------------------------------------*
 *  Main sequence function                                             *
 *  a(idx_t n) -> seq_t                                                *
 *---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* a(0) is the empty sum */
    if (n == 0)
        return (seq_t)0;

    /* Determine how many bits the binary representation of n uses */
    size_t bits = 0;
    idx_t  tmp  = n;
    while (tmp) {
        bits++;
        tmp >>= 1;
    }

    /* Ensure we have odd Fibonacci numbers for all bit positions needed */
    size_t needed = bits - 1;           /* highest bit index in n */
    ensure_odd_fibs(needed);

    /* Sum the selected odd Fibonacci numbers */
    seq_t sum  = 0;
    idx_t mask = n;
    size_t i = 0;
    while (mask) {
        if (mask & 1)
            sum += odd_fibs[i];
        mask >>= 1;
        i++;
    }
    return sum;
}
