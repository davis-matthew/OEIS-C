/*
 * A217584  Numbers k such that d(k^2)/d(k) is an integer, where d(k) is the
 *          number of divisors of k.
 * https://oeis.org/A217584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS A217584.
   A term k is included iff k = m² and the ratio d(k²)/d(k) is an
   integer.  The function enumerates m, tests the condition for the
   corresponding k, and memoises the sequence in a static array.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Simple Euclidean gcd for 64‑bit unsigned integers. */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Maximum number of terms that will be stored. 10000 is enough for
   the required range; an arbitrary‑precision build will still
   handle the values. */
#define MAX_TERMS 20000

/* Up to MAX_TERMS terms are kept in a static array.  For fixed‑width
   types this is trivial; for an arbitrary‑precision type the array
   holds default‑constructed objects, which is always legal. */
static seq_t  seq[MAX_TERMS];
static size_t seq_len = 0;   /* number of terms already stored */

/* The next integer m to test (k = m²).  It is global so that calls
   to a() in arbitrary order keep progressing the enumeration. */
static uint64_t next_m = 1;

/* Determine whether the square k = m² satisfies the required
   divisibility condition.  We factor m and maintain the product
   of (4·e+1) and (2·e+1) where e is the exponent of each prime.  */
static bool is_valid_m(uint64_t m)
{
    uint64_t temp = m;
    uint64_t num  = 1;
    uint64_t den  = 1;

    /* trial division; m is small enough that this is fast. */
    for (uint64_t p = 2; p * p <= temp; p += (p == 2) ? 1 : 2) {
        uint64_t e = 0;
        while ((temp % p) == 0) {
            temp /= p;
            e++;
        }
        if (e > 0) {
            num *= (4 * e + 1);
            den *= (2 * e + 1);
            uint64_t g = gcd_uint64(num, den);
            if (g > 1) {
                num /= g;
                den /= g;
            }
        }
    }
    if (temp > 1) {   /* remaining prime factor, exponent 1 */
        num *= 5;     /* 4·1+1 */
        den *= 3;     /* 2·1+1 */
        uint64_t g = gcd_uint64(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }
    }
    return (den == 1);
}

/* Append a new term to the sequence array.  We rely on the
   fixed capacity defined by MAX_TERMS. */
static void add_term(seq_t val)
{
    if (seq_len < MAX_TERMS) {
        seq[seq_len++] = val;
    }
}

/* ------------------------------------------------------------ */
/* Public sequence function required by the interface. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for indices < 1 */

    /* Generate terms until we have at least n of them. */
    size_t need = (size_t)n;
    while (seq_len < need) {
        if (is_valid_m(next_m)) {
            /* k = m²; cast to seq_t for arbitrary‑precision builds. */
            seq_t k = (seq_t)next_m * (seq_t)next_m;
            add_term(k);
        }
        next_m++;
    }
    return seq[n - 1];
}
