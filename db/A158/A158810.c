/*
 * A158810  Coefficients of the differentiated row polynomials of the
 *          triangular Hadamard matrices of A158800: p(x,n)=If[n less than or
 *          equal to 2^m,Sum[H(2^m)[[k]]*x^(1-k),{k,1,n}],If[n greater than m
 *          then m+1].
 * https://oeis.org/A158810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, uned
 * Formula:   not stated
 * Range:     i8 n <= 89, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A158810 – coefficients of the differentiated row polynomials 
   of the triangular Hadamard matrices of A158800.
   The n-th term a(n) is given by:
     a(n) = 0                     if n == 0
     else let r be the least integer with r*(r+1)/2 >= n
          let l = n - (1 + (r-1)r/2) + 1   (1‑based position inside row r)
          if (l & ~r) != 0      -> a(n) = 0
          else                  -> a(n) = (-1)^{popcount(l)} * l
   This formula reproduces the listed terms and works for any n >= 0.
*/

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root (floor).  Uses only integer arithmetic. */
static unsigned long long isqrt_u64(unsigned long long n)
{
    unsigned long long x = 0;
    unsigned long long bit = 1ULL << 62; /* highest power of two square that fits */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

/* Main sequence function.  External linkage is required. */
seq_t a(idx_t n)
{
    /* Compute r: the least integer with r*(r+1)/2 >= n. */
    unsigned long long r = (isqrt_u64(1 + 8ULL * n) - 1) / 2;
    /* Adjust in case the approximation was too low. */
    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    while (tri < n) {
        ++r;
        tri = (unsigned __int128)r * (r + 1) / 2;
    }

    /* Offset of first element of row r. */
    unsigned __int128 prev = (r == 0) ? 0 :
                           (unsigned __int128)(r - 1) * r / 2;
    unsigned __int128 offsetPrev = 1 + prev;

    /* Position l within row r (1‑based). */
    unsigned __int128 l128 = (unsigned __int128)n - offsetPrev + 1;
    unsigned long long l = (unsigned long long)l128;

    /* If l is not a sub‑mask of r, the value is zero. */
    if ( (l & ~r) != 0 )
        return 0;

    /* Sign determined by parity of popcount(l). */
    unsigned int pc = __builtin_popcountll(l);
    seq_t val = (seq_t)l;
    if (pc & 1)
        val = -val;
    return val;
}
