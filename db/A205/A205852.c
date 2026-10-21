/*
 * A205852  Numbers k for which 5 divides s(k)-s(j) for some j<k; each k
 *          occurs once for each such j; s(k) denotes the (k+1)-st Fibonacci
 *          number.
 * https://oeis.org/A205852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205852:
   a(n) is the n-th element in the multiset in which each integer k
   (k ≥ 1) appears as many times as there are j < k with
   F_{k+1} ≡ F_{j+1} (mod 5), where F_m are the Fibonacci numbers
   (F₁ = 1, F₂ = 1).  The code counts residues of Fibonacci numbers
   modulo 5 and accumulates occurrences until the requested index n
   is reached.                                    */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* counts of seen indices for each residue 0‑4 */
    idx_t count[5] = {0, 0, 0, 0, 0};

    /* Fibonacci numbers modulo 5, starting with F₁ and F₂ */
    seq_t fib_a = 1; /* F₁ */
    seq_t fib_b = 1; /* F₂ */

    idx_t cumulative = 0;     /* number of terms already produced */

    for (idx_t k = 1; ; ++k) {
        /* current residue r = F_{k+1} mod 5 */
        int r = (int)(fib_b % 5);

        idx_t occ = count[r];          /* how many j < k share this residue */

        if (occ > 0) {
            if (cumulative + occ >= n) {
                return (seq_t)k;       /* n‑th term is k */
            }
            cumulative += occ;
        }

        /* update for next k */
        count[r] += 1;

        /* advance Fibonacci modulo 5: F_{m+1} = (F_{m-1} + F_{m}) mod 5 */
        seq_t next = (fib_a + fib_b) % 5;
        fib_a = fib_b;
        fib_b = next;
    }
}
