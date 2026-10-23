/*
 * A106990  Primes of the form 5x^2+5xy+6y^2, with x and y nonnegative.
 * https://oeis.org/A106990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A106990 – Primes of the form 5x^2 + 5xy + 6y^2 (x,y ≥ 0)  
   The function finds the nth such prime by testing successive odd numbers
   for primality and representability.  No explicit recurrence or
   closed‑form is known, so a simple routine with a primality test and a
   direct integer‑square‑root check suffices for the required range.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- small utilities used only in this file ---------- */

/* fast modular multiplication for 64‑bit values (uses 128‑bit) */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t m) {
    return (unsigned __int128)a * b % m;
}

/* fast modular exponentiation: base^exp mod mod */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    uint64_t cur = base % mod;
    while (exp) {
        if (exp & 1) res = mod_mul64(res, cur, mod);
        cur = mod_mul64(cur, cur, mod);
        exp >>= 1;
    }
    return res;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        uint64_t a = small[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* integer square root for 128‑bit numbers (floor) */
static uint64_t isqrt128(unsigned __int128 n) {
    unsigned __int128 lo = 0, hi = n, mid, r = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        if (mid == 0 || mid <= n / mid) {
            r = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return (uint64_t)r;
}

/* test if a prime p can be written as 5x^2 + 5xy + 6y^2 with x,y≥0 */
static bool is_represented(uint64_t p) {
    /* y max: 20p - 95y^2 ≥ 0  →  y ≤ sqrt(20p/95) */
    unsigned __int128 bound_num = (unsigned __int128)20 * p;
    unsigned __int128 bound_den = 95;
    uint64_t y_max = isqrt128(bound_num / bound_den);

    for (uint64_t y = 0; y <= y_max; ++y) {
        unsigned __int128 yy = (unsigned __int128)y * y;
        unsigned __int128 D  = (unsigned __int128)20 * p - (unsigned __int128)95 * yy;
        uint64_t s = isqrt128(D);
        if ((unsigned __int128)s * s != D) continue;          /* not a perfect square */
        if (s < 5 * y) continue;                              /* would give negative x   */
        uint64_t diff = s - 5 * y;
        if (diff % 10 != 0) continue;                         /* parity of x must be integer */
        /* x = diff / 10 is non‑negative integer → representation holds */
        return true;
    }
    return false;
}

/* ---------- sequence storage ----------------------------------- */

static seq_t *seq = NULL;      /* dynamic array of found terms          */
static idx_t  seq_size = 0;    /* number of elements currently stored  */
static idx_t  seq_cap  = 0;    /* current capacity                      */

/* grow the array if necessary and append a new term     */
static void add_term(seq_t val) {
    if (seq_size == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        seq_t *newp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = newp;
        seq_cap = (idx_t)new_cap;
    }
    seq[seq_size++] = val;
}

/* ---------- required function --------------------------------- */

seq_t a(idx_t n) {
    /* generate terms until we have at least n of them */
    static uint64_t candidate = 5;  /* first prime of the form 5x^2+5xy+6y^2 */
    while (seq_size < n) {
        if (is_prime_uint64(candidate) && is_represented(candidate))
            add_term((seq_t)candidate);
        candidate += 2;     /* skip even numbers after 5 */
    }
    return seq[n - 1];
}
