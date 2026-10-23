/*
 * A186518  Triangle A177517 mod 2. Mahonian numbers modulo 2.
 * https://oeis.org/A186518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   T(1,1)          = 1
   T(n,1)          = 0  (n > 1)
   T(n,k)          = ( Σ_{i=1..k-1} T(n-i,k-1) ) mod 2   (k > 1)

   The sequence a(n) is the triangle read in row‑major order (columns
   from 1 to the row number).  The function below computes a(n) for any
   n ≥ 1 by generating the triangle up to the required row.
*/
#include <stddef.h>
#include <stdint.h>

#define MAX_TRI 20000   /* sufficient for n ≤ 10000 (row ≤ 140) */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1 */

    /* Find the row r that contains position n */
    idx_t r = 1;
    while ( (r * (r + 1)) / 2 < n ) ++r;

    /* Triangle storage: tri[idx] holds T(row,col) with
       idx = row*(row-1)/2 + (col-1)   (0‑based linear index) */
    static seq_t tri[MAX_TRI];

    /* Build the triangle up to row r */
    for (idx_t row = 1; row <= r; ++row) {
        for (idx_t col = 1; col <= row; ++col) {
            idx_t idx = row * (row - 1) / 2 + (col - 1);

            if (col == 1) {
                tri[idx] = (row == 1) ? 1 : 0;
            } else {
                seq_t sum = 0;
                /* i must satisfy row-i ≥ col-1; thus i ≤ row-(col-1) */
                idx_t max_i = col - 1;
                idx_t possible = row - (col - 1);
                if (possible < max_i) max_i = possible;

                for (idx_t i = 1; i <= max_i; ++i) {
                    idx_t prev_row = row - i;
                    idx_t prev_idx = prev_row * (prev_row - 1) / 2 + (col - 2);
                    sum ^= tri[prev_idx];  /* addition modulo 2 */
                }
                tri[idx] = sum;
            }
        }
    }

    /* Convert the one‑based position n to a zero‑based linear index. */
    return tri[n - 1];
}
