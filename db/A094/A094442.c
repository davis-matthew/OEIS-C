/*
 * A094442  Triangular array T(n,k) = Fibonacci(n+2-k)*C(n,k), 0 <= k <= n.
 * https://oeis.org/A094442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 95, u32 n <= 332, u64 n <= 1239,
 *            u128 n <= 4592, big any n
 * Helpers:   binom_id, fib_id
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the term a(n) of OEIS A094442.
   The sequence is the row‑wise flattening of the triangular array
   T(n,k) = C(n,k) * Fibonacci(n+2−k),  0 ≤ k ≤ n,
   where Fibonacci(0) = 0, Fibonacci(1) = 1, Fibonacci(2) = 1, …
   The flattened index is: a(0) = T(0,0), a(1) = T(1,0), a(2) = T(1,1), … */

#include <stdint.h>
#include "../utils/binom_id.h"
#include "../utils/fib_id.h"

/* ---------- Find the row r that contains the flattened index n -------- */
static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    /* while r*(r+1)/2 ≤ n < (r+1)*(r+2)/2  -- all arithmetic in idx_t */
    while ((r * (r + 1)) / 2 <= n) r++;
    return r - 1;
}

/* ---------- Main function ---------------------------------------------- */
seq_t a(idx_t n)
{
    /* Map the flat index n to row r and column k (k=0…r) */
    idx_t r = find_row(n);
    idx_t start_of_row = (r * (r + 1)) / 2;
    idx_t k = n - start_of_row;          /* 0 ≤ k ≤ r */

    /* Compute the needed Fibonacci number: Fib(r + 2 – k) */
    idx_t fib_index = r + 2 - k;
    seq_t fib_val = fib_id(fib_index);

    /* Compute the binomial coefficient C(r,k) */
    seq_t binom_val = binom_id(r, k);

    /* Return the product as defined for T(r,k) */
    return fib_val * binom_val;
}
