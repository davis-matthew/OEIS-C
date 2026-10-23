/*
 * A175286  Pisano period of the Jacobsthal sequence A001045 modulo n.
 * https://oeis.org/A175286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the eventual cycle of the Jacobsthal sequence
 *            modulo n. The Jacobsthal sequence is J(0)=0, J(1)=1,
 *            J(k)=J(k-1)+2*J(k-2)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef struct {
    seq_t a, b;
} jp_pair;

static inline jp_pair next_pair(jp_pair p, uint64_t mod)
{
    /* Compute J(next) = J(k)+2*J(k-1) modulo mod safely. */
    unsigned long long tmp = (unsigned long long)p.a * 2 + (unsigned long long)p.b;
    jp_pair r;
    r.a = p.b;
    r.b = (seq_t)(tmp % mod);
    return r;
}

seq_t a(idx_t n)
{
    /* Handle the trivial case n=0, though the sequence starts at 1. */
    if (n == 0) return 0;

    uint64_t mod = (uint64_t)n;

    jp_pair state0;
    state0.a = 0;
    state0.b = (seq_t)(1 % mod);  /* 1 mod n, equals 0 if n==1 */

    jp_pair tortoise = next_pair(state0, mod);
    jp_pair hare      = next_pair(next_pair(state0, mod), mod);

    while (tortoise.a != hare.a || tortoise.b != hare.b) {
        tortoise = next_pair(tortoise, mod);
        hare = next_pair(next_pair(hare, mod), mod);
    }

    /* Determine the length of the cycle. */
    uint64_t lam = 1;
    jp_pair hare2 = next_pair(tortoise, mod);
    while (tortoise.a != hare2.a || tortoise.b != hare2.b) {
        hare2 = next_pair(hare2, mod);
        lam++;
    }

    return (seq_t)lam;
}
