/*
 * A133194  Numbers requiring a prime number of chisel strokes for their
 *          representation in Roman numerals.
 * https://oeis.org/A133194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 1215, u32 n <= 1215, u64 n <= 1215,
 *            u128 n <= 1215, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose Roman numeral representation requires a prime
   number of chisel strokes.  Computed by brute‑force search: for each
   integer, the Roman numeral is converted, the stroke count is
   computed, and the count is tested for primality.  The first such
   integer encountered is returned for each index `n`. */

#include <stdint.h>
#include <stdbool.h>

/* Stroke counts for base symbols in the units, tens, hundreds and
   thousands groups (the pattern repeats every 4 decimal positions).
   base1  = value of the 1× (I, X, C, M, ...),
   base5  = value of the 5× (V, L, D, …),
   base10 = value of the 10× (X, C, M, …).  */
static const uint64_t base1[4]  = {1, 2, 2, 4};
static const uint64_t base5[4]  = {2, 2, 3, 2};
static const uint64_t base10[4] = {2, 2, 4, 2};

/* Compute the Roman‑numeral stroke count for a positive integer.
   The algorithm processes each decimal digit and uses the standard
   Roman‑numeral construction rules. */
static uint64_t roman_strokes(uint64_t num)
{
    uint64_t strokes = 0;
    uint64_t pos = 0;          /* 0 = units, 1 = tens, …            */
    while (num > 0) {
        uint64_t d = num % 10; /* current digit                      */
        if (d) {
            uint64_t b = pos & 3; /* pos % 4, indices into the base arrays */
            if (d <= 3)
                strokes += d * base1[b];
            else if (d == 4)
                strokes += base1[b] + base5[b];
            else if (d == 5)
                strokes += base5[b];
            else if (d <= 8)
                strokes += base5[b] + (d - 5) * base1[b];
            else /* d == 9 */
                strokes += base1[b] + base10[b];
        }
        num /= 10;
        pos++;
    }
    return strokes;
}

/* Primality test for 64‑bit integers using 6k±1 optimisation.  */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    if (n % 3 == 0)
        return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

/* Main sequence function as required by the judge. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, return 0    */
    idx_t count = 0;
    uint64_t value = 2;           /* a(1) = 2                         */
    for (;; value++) {
        if (is_prime_uint64(roman_strokes(value))) {
            count++;
            if (count == n)
                return (seq_t)value;
        }
    }
}
