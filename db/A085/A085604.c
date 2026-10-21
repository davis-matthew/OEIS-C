/*
 * A085604  T(n,k) = highest power of prime(k) dividing n!, read by rows.
 * https://oeis.org/A085604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = exponent of the prime with index k in (row)!, * where n
 *            is mapped to the pair (row, col) by traversing the * triangular
 *            table row‑by‑row. The exponent is computed via * Legendre's
 *            formula
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   decode_row, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/decode_row.h"

/* Legendre's formula: return exponent of prime p in n!. */
static seq_t exponent_in_factorial(idx_t n, uint64_t p)
{
    seq_t exp = 0;
    uint64_t t = n;
    while (t) {
        t /= p;
        exp += t;
    }
    return exp;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1; safe guard for invalid input */
        return 0;

    idx_t row, col;
    decode_row(n, &row, &col);

    uint64_t prime = get_prime(col);   /* 1‑based index: prime(col) */
    return exponent_in_factorial(row, prime);
}
