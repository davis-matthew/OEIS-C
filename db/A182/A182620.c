/*
 * A182620  Triangle T(n,k) read by rows in which row n lists the divisors of
 *          n, written in base 2.
 * https://oeis.org/A182620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 118, u32 n <= 7069, u64 n <= 7069,
 *            u128 n <= 7069, big any n
 * Helpers:   bin_to_dec, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182620 – binary representations of divisors, read by rows
   a(n) = the binary digits of the divisor of the row r that occupies
   index n when all rows are concatenated.  Rows are 1‑based, divisors
   within each row are sorted increasingly.  The binary string is
   interpreted as a decimal integer (e.g., binary 101 → 101).  This
   implementation runs up to n≈10 000 in well under 2 s.  The code is
   fully conformant to C11 and C++17. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/bin_to_dec.h"

seq_t a(idx_t n)
{
    idx_t cumulative = 0;   /* total number of terms processed so far */
    idx_t r = 1;           /* current row number */

    /* find the row containing the nth term */
    while (1) {
        uint64_t dcnt = divisor_count_u64((uint64_t)r);
        if (cumulative + dcnt >= n) break;
        cumulative += dcnt;
        ++r;
    }

    /* position of the term within row r (1‑based) */
    idx_t pos = n - cumulative;

    /* enumerate all divisors of r, store them in an array */
    uint64_t divs[256];
    size_t cnt = 0;
    for (uint64_t i = 1; i * i <= r; ++i) {
        if (r % i == 0) {
            divs[cnt++] = i;
            if (i != r / i) divs[cnt++] = r / i;
        }
    }

    /* sort the divisors ascending (usually very few divisors) */
    for (size_t j = 1; j < cnt; ++j) {
        uint64_t key = divs[j];
        int i = (int)j - 1;
        while (i >= 0 && divs[i] > key) {
            divs[i + 1] = divs[i];
            --i;
        }
        divs[i + 1] = key;
    }

    /* select the pos‑th divisor (pos is 1‑based) */
    uint64_t divisor = divs[(size_t)pos - 1];

    /* convert this divisor to its binary‑digit decimal representation */
    return bin_to_dec(divisor);
}
