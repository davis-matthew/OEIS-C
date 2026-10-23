/*
 * A175290  Pisano period of A030195 modulo n.
 * https://oeis.org/A175290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* \tSequence: Pisano period of A030195 modulo n
 *
 * \tA030195 is defined by a(0)=0, a(1)=1 and
 * \ta(n) = 3*(a(n-1)+a(n-2)) for n>=2.
 *
 * \ta(n) returns the length of the eventual cycle of the recurrence,
 * computed modulo the argument `n` (with n>=1).  The period is the
 * number of distinct states (a(k),a(k+1)) modulo n that form the cycle.
 * The computation uses Floyd’s cycle–finding algorithm and requires
 * only a handful of integer operations, so it is efficient for all
 * n up to at least 10 000.
 */

#include <stdbool.h>

typedef struct { seq_t a; seq_t b; } pair_t;

/* Advance the pair once according to the recurrence modulo `mod`. */
static inline void advance_pair(pair_t *p, seq_t mod)
{
    /* Convert to 64‑bit for intermediate arithmetic; values < mod <= 10000. */
    uint64_t mod64 = (uint64_t)mod;
    uint64_t a = (uint64_t)p->a;
    uint64_t b = (uint64_t)p->b;

    uint64_t a_next = b;

    /* (a + b) mod mod */
    uint64_t sum = a + b;
    if (sum >= mod64) sum -= mod64;

    /* 3 * sum mod mod */
    uint64_t prod = sum * 3ULL;
    if (prod >= mod64) prod %= mod64;

    p->a = (seq_t)a_next;
    p->b = (seq_t)prod;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Modulus must be positive; n>=1 by sequence offset. */
    seq_t mod = (seq_t)n;

    /* Initial state (a0,a1) modulo n */
    pair_t start = { 0, (seq_t)(mod == 0 ? 0 : (1 % mod)) };

    /* ---- Floyd's cycle detection ---- */
    pair_t tortoise = start, hare = start;

    advance_pair(&tortoise, mod);          /* f(start)      */
    advance_pair(&hare, mod);              /* f(start)      */
    advance_pair(&hare, mod);              /* f(f(start))   */

    while (!(tortoise.a == hare.a && tortoise.b == hare.b)) {
        advance_pair(&tortoise, mod);
        advance_pair(&hare, mod);
        advance_pair(&hare, mod);
    }

    /* Find the start of the cycle (mu). */
    pair_t mu_state = start;
    while (!(mu_state.a == tortoise.a && mu_state.b == tortoise.b)) {
        advance_pair(&mu_state, mod);
        advance_pair(&tortoise, mod);
    }

    /* Find the cycle length (lambda). */
    pair_t hare2 = tortoise;          /* already at mu_state */
    advance_pair(&hare2, mod);        /* f(mu_state)    */
    seq_t lambda = 1;
    while (!(hare2.a == mu_state.a && hare2.b == mu_state.b)) {
        advance_pair(&hare2, mod);
        ++lambda;
    }

    return lambda;
}
