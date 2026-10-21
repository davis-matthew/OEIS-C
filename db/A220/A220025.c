/*
 * A220025  The period with which the powers of n repeat mod 100000.
 * https://oeis.org/A220025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the minimal period of the sequence n^i (mod 100000) i.e.
 *            the smallest positive integer k such that n^(i+k) ≡ n^i (mod
 *            100000) for all sufficiently large i. For n=0 the sequence is
 *            1,0,0,… so the period is 1; for n coprime to 10 the period is
 *            the multiplicative order modulo 100000, for othe...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)100000;          /* modulus 10^5          */
    seq_t nmod = (seq_t)n % MOD;              /* reduce base          */

    /* Floyd cycle detection on the sequence v_0 = 1, v_{i+1} = v_i * nmod (mod MOD) */

    seq_t tortoise, hare, cur;

    /* first step: v_1 = 1 * nmod % MOD */
    tortoise = nmod;
    /* second step: v_2 = v_1 * nmod % MOD */
    hare = (tortoise * nmod) % MOD;

    while (tortoise != hare) {
        tortoise = (tortoise * nmod) % MOD;      /* advance by 1   */
        hare = (hare * nmod) % MOD;              /* advance by 1   */
        hare = (hare * nmod) % MOD;              /* advance by another 1 -> 2 total  */
    }

    /* Find cycle length λ */
    seq_t lambda = 1;
    cur = (tortoise * nmod) % MOD;
    while (cur != tortoise) {
        cur = (cur * nmod) % MOD;
        lambda++;
    }

    return lambda;
}
