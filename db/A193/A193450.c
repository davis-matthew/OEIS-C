/*
 * A193450  Triangle of a binomial convolution sum related to Jacobsthal
 *          numbers.
 * https://oeis.org/A193450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A193450 – Triangle of a binomial convolution sum related to Jacobsthal numbers.
 * The function a(n) returns the n-th term in the concatenated triangle.  For index n,
 * locate the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.  Let k = n - r*(r+1)/2.
 * Then a(n) = T(r,k) where 
 *     T(r,k) = Σ_{j=0..k} (-1)^j * r * C(r-j, k-j).
 * Rather than computing binomial coefficients explicitly, we use the recurrence
 *     term_j = -( (k-j)/(n-j) ) * term_{j-1}  (j ≥ 1),
 * which only involves integer divisions that are exact.  The computation starts
 * from term_k = (-1)^k * r and runs backward to j=0.
 *
 * Complexity: O(sqrt(n)) per call, well under the time limit for n up to 10 000.
 * No external libraries are needed beyond the provided binary coefficient function.
 */

#include <stdint.h>
#include <stdbool.h>

/* Find the row index r for which r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    unsigned __int128 count = 0;          /* total elements before the next row */
    while (count + (r + 1) <= (unsigned __int128)n) {
        count += r + 1;
        ++r;
    }
    return r;
}

/* Compute a(n) as defined above. */
seq_t a(idx_t n)
{
    /* Locate the row and column in the triangle. */
    idx_t r = find_row(n);                     /* row index */
    unsigned __int128 cnt = (unsigned __int128)r * (r + 1U) / 2U; /* elements before row r */
    idx_t k = n - (idx_t)cnt;                  /* column index in row r */

    /* Apply the backward recurrence for the terms of T(r,k). */
    unsigned long long term_mag = (unsigned long long)r;     /* magnitude of the current term */
    bool sign_pos = ((k & 1U) == 0U);      /* true if the term is positive */
    long long sum = 0;                     /* accumulate the sum in a signed type */

    for (idx_t j = k;; --j) {
        if (sign_pos)
            sum += (long long)term_mag;
        else
            sum -= (long long)term_mag;

        if (j == 0)
            break;

        /* Compute the magnitude of the next term (j-1). */
        unsigned long long num = r - j + 1U;
        unsigned long long den = k - j + 1U;
        unsigned __int128 prod = (unsigned __int128)term_mag * num;
        term_mag = (unsigned long long)(prod / den);

        sign_pos = !sign_pos;  /* toggle the sign for the next term */
    }

    return (seq_t)sum;
}
