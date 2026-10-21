/*
 * A213277  a(n) is the length of the cycle of first differences of k such
 *          that Fibonacci(k) mod n = k mod n.
 * https://oeis.org/A213277
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 9784, u32 n <= 9784, u64 n <= 9784,
 *            u128 n <= 9784, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A213277
   a(n) = length of the cycle of the first differences
          of the integers k satisfying Fib(k) ≡ k (mod n).
   For each n ≥ 2 this function returns the exact value. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"           /* gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* sequence is defined only for n ≥ 2 */

    /* ---------- Pisano period π(n) ---------- */
    seq_t nn = (seq_t)n;          /* n as seq_t for modulus operations */
    seq_t prev = 0;
    seq_t curr = 1 % nn;
    size_t period = 0;
    do {
        seq_t next = prev + curr;
        if (next >= nn) next -= nn; /* keep within modulo n */
        prev = curr;
        curr = next;
        ++period;
    } while (!(prev == 0 && curr == 1));   /* first repetition of (0,1) */

    /* ---------- compute gcd(π(n), n) ---------- */
    seq_t g_seq = gcd((seq_t)period, nn);
    idx_t g = (idx_t)g_seq;      /* g ≥ 1 */

    /* ---------- count residues r < π(n) with Fib(r) ≡ r (mod g) ----------
       We generate Fibonacci numbers modulo g on the fly.
    */
    seq_t fib = 0;                     /* Fib(0) mod g */
    seq_t fib_next = 1 % g_seq;        /* Fib(1) mod g */
    idx_t r_mod_g = 0;                 /* r mod g */
    size_t count_residues = 0;
    for (idx_t r = 0; r < period; ++r) {
        if (fib == (seq_t)r_mod_g) ++count_residues;

        /* prepare numbers for next Fibonacci term */
        seq_t next = fib + fib_next;
        if (next >= g_seq) next -= g_seq;   /* modulo g */
        fib = fib_next;
        fib_next = next;

        /* increment r mod g */
        ++r_mod_g;
        if (r_mod_g == g) r_mod_g = 0;
    }

    /* ---------- result ---------- */
    return (seq_t)count_residues;
}
