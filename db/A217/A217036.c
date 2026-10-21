/*
 * A217036  Term preceding the first zero in the Fibonacci numbers modulo n.
 * https://oeis.org/A217036
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = F_{G(n)-1} mod n,
 * where G(n) is the least positive integer k such that the k‑th Fibonacci number F_k is
 * divisible by n.  This implementation iterates Fibonacci numbers modulo n until the
 * first zero occurs, then returns the preceding Fibonacci value.
 */

#include <stdint.h>

static inline seq_t mod_add(seq_t a, seq_t b, seq_t mod)
{
    seq_t sum = a + b;
    if (sum < a || sum >= mod)   /* handle wrap‑around for built‑in integer types */
        sum -= mod;
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* sequence is defined for n >= 2 */

    const seq_t mod = (seq_t)n;   /* cast once to seq_t for all operations */
    seq_t f_prev = 0;             /* F_0 mod n */
    seq_t f_curr = 1 % mod;       /* F_1 mod n */

    for (;;)
    {
        if (f_curr == 0)         /* first zero encountered – G(n) found */
            return f_prev;       /* F_{G(n)-1} mod n */

        seq_t next = mod_add(f_prev, f_curr, mod); /* F_{k+1} mod n */
        f_prev = f_curr;
        f_curr = next;
    }
}
