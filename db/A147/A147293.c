/*
 * A147293  Triangle read by rows, A011782 convolved with A001519.
 * https://oeis.org/A147293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read row‑wise from the lower‑triangular matrix *
 *            A147293(i,j) = A011782(i−j) · F_{2j+1}, * where F_m is the m‑th
 *            Fibonacci number (F_0 = 0, F_1 = 1). * A011782(k) = 1 for k =
 *            0,1; otherwise 2^(k−1). * For a given linear index n (offset 0),
 *            let * r = floor((sqrt(8n+1)−1)/2) // row number *...
 * Range:     u8 n <= 34, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/fib.h"
#include "../utils/pow2.h"

static inline idx_t row_for_n(idx_t n) {
    /* Compute r = floor((sqrt(8n+1)-1)/2) with corrections. */
    unsigned __int128 tmp = (unsigned __int128)8 * n + 1;
    long double val = sqrtl((long double)tmp);
    idx_t r = (idx_t)((val - 1.0L) / 2.0L);
    /* Correct possible round‑off errors. */
    while (((__int128)r * (r + 1)) / 2 > n) --r;
    while (((__int128)(r + 1) * (r + 2)) / 2 <= n) ++r;
    return r;
}

seq_t a(idx_t n) {
    idx_t r = row_for_n(n);
    idx_t row_start = r * (r + 1) / 2;
    idx_t c = n - row_start;          /* column index (0‑based) */
    idx_t k = r - c;                  /* offset in A011782 */

    seq_t seq_val;
    if (k <= 1)
        seq_val = (seq_t)1;
    else
        seq_val = pow2(k - 1);        /* 2^(k-1) */

    seq_t fib_val = fib(2 * c + 1);   /* odd‑indexed Fibonacci */

    return seq_val * fib_val;
}
