/*
 * A192579  Primes p for which there is no prime q == 3 (mod 4) that is
 *          smaller than p and is a quadratic residue modulo p.
 * https://oeis.org/A192579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: full, pass, 5 terms through n = 5 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A192579 – primes p for which no smaller prime q ≡ 3 (mod 4) is a quadratic
    residue modulo p.  By Gica’s result the sequence is finite and consists of
    exactly the primes 2, 3, 5, 7, 17.  Therefore a(n) is 0 for n > 5. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    /*  Offset 1.  a(1) = 2, a(2) = 3, a(3) = 5, a(4) = 7, a(5) = 17. */
    static const seq_t values[5] = {
        (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)7, (seq_t)17
    };

    if (n == 0) return (seq_t)0;        /* never used – offset is 1 */
    if (n <= 5) return values[n-1];
    return (seq_t)0;                    /* no further terms */
}
