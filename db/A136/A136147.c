/*
 * A136147  Length of the cycles in which finish the sequences a(k+1)=sopfr(n
 *          a(k)+ D), with sopfr=A001414 and D=2. Similar to A136140 which has
 *          D=1.
 * https://oeis.org/A136147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the length of the cycle reached by iterating f(x) =
 *            sopfr(n·x + 2), starting from x = 1. Here sopfr is the sum of
 *            prime factors with multiplicity (OEIS A001414). The
 *            implementation uses Floyd's cycle‑finding algorithm and trial
 *            division for the factorisation
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 39, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Sum of prime factors with multiplicity (sopfr) for a seq_t value. */
/* ------------------------------------------------------------------ */
static seq_t sopfr_seq(seq_t v)
{
    if (v <= (seq_t)1) return (seq_t)0;

    seq_t sum = (seq_t)0;

    /* factor 2 */
    while (v % (seq_t)2 == (seq_t)0) {
        sum += (seq_t)2;
        v   /= (seq_t)2;
    }

    /* factor odd primes */
    seq_t p = (seq_t)3;
    while (p * p <= v) {
        if (v % p == (seq_t)0) {
            while (v % p == (seq_t)0) {
                sum += p;
                v   /= p;
            }
        }
        p += (seq_t)2;          /* only odd candidates */
    }

    /* if v > 1 then v itself is prime */
    if (v > (seq_t)1) sum += v;

    return sum;
}

/* ------------------------------------------------------------------ */
/*  Perform one step of the map f(x) = sopfr(n·x + 2).                 */
/* ------------------------------------------------------------------ */
static seq_t step(seq_t x, seq_t n)
{
    seq_t val = n * x + (seq_t)2;
    return sopfr_seq(val);
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Convert n to seq_t once for reuse in product computation. */
    seq_t n_seq = (seq_t)n;

    /* Floyd's cycle‑finding algorithm. */
    seq_t tortoise = step((seq_t)1, n_seq);
    seq_t hare     = step(step((seq_t)1, n_seq), n_seq);

    while (tortoise != hare) {
        tortoise = step(tortoise, n_seq);
        hare     = step(step(hare, n_seq), n_seq);
    }

    /* Find start of cycle (mu). */
    seq_t mu = (seq_t)1;
    while (mu != hare) {
        mu = step(mu, n_seq);
        hare = step(hare, n_seq);
    }

    /* Find length of cycle (lambda). */
    seq_t lam = (seq_t)1;
    seq_t next = step(mu, n_seq);
    while (mu != next) {
        next = step(next, n_seq);
        lam++;
    }

    return lam;
}
