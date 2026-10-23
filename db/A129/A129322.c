/*
 * A129322  Triangle where the n-th row lists the smallest n positive integers
 *          which are coprime to the n-th Fibonacci number.
 * https://oeis.org/A129322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1827, u16 n <= 9959, u32 n <= 9959, u64 n <= 9959,
 *            u128 n <= 9959, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the nth term of the flattened version of the triangle
 * where row r contains the r smallest positive integers coprime to the r-th
 * Fibonacci number F_r (with F_1 = 1, F_2 = 1).  For a given n the function
 * finds the row r with (r-1)r/2 < n ≤ r(r+1)/2, sets pos = n - (r-1)r/2,
 * and returns the pos‑th smallest integer coprime to F_r.
 * 
 * The implementation uses fast doubling to compute F_r modulo a small
 * integer m, so that only 64‑bit arithmetic is required for the coprimality
 * test.  No external libraries are needed.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper: gcd for unsigned 64‑bit integers --------------------------------*/
static unsigned long long gcd_ull(unsigned long long a, unsigned long long b)
{
    while (b != 0) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* helper: Fibonacci(F_n) mod m using fast doubling ------------------------*/
static uint64_t fib_mod_u64(uint64_t n, uint64_t mod)
{
    if (n == 0) return 0;                 /* F_0 = 0 */
    uint64_t a = 0;                       /* F_k   */
    uint64_t b = 1;                       /* F_{k+1} */
    int msb = 63 - __builtin_clzll(n);
    for (int i = msb; i >= 0; --i) {
        /* c = a * (2*b - a)  (mod mod) */
        uint64_t twob = (b << 1) % mod;
        uint64_t c = (a * ((twob + mod - a) % mod)) % mod;
        /* d = a*a + b*b      (mod mod) */
        uint64_t d = ((a * a) % mod + (b * b) % mod) % mod;
        a = c;
        b = d;
        if ((n >> i) & 1ULL) {            /* if bit set, advance one step */
            uint64_t e = (a + b) % mod;    /* F_{k+1} becomes F_k+F_{k+1} */
            a = b;
            b = e;
        }
    }
    return a;  /* F_n mod mod */
}

/* -------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1, undefined below */

    /* Find the row r such that (r-1)r/2 < n ≤ r(r+1)/2 */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) r++;

    idx_t previous = (r - 1) * r / 2;
    idx_t pos = n - previous;             /* desired position in row r */

    idx_t count = 0;
    idx_t m = 1;                          /* candidate integer */

    while (1) {
        uint64_t mod = (uint64_t)m;
        uint64_t fmod = fib_mod_u64((uint64_t)r, mod);   /* F_r mod m */
        if (gcd_ull((unsigned long long)m, (unsigned long long)fmod) == 1ULL) {
            ++count;
            if (count == pos)
                return (seq_t)m;
        }
        ++m;
    }
}
