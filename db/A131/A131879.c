/*
 * A131879  Triangle where row n contains the first n positive integers which
 *          are coprime to (2^n -1).
 * https://oeis.org/A131879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the nth term of the triangle whose nth row
 * contains the first n positive integers coprime to (2^n - 1).
 * The sequence is read row‑by‑row (row 1, then row 2, …).
 */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean gcd for unsigned 64‑bit integers */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* 2^exp mod mod_val (mod_val > 0) */
static inline uint64_t modpow2_uint64(uint64_t exp, uint64_t mod_val)
{
    uint64_t res = 1 % mod_val;
    for (uint64_t i = 0; i < exp; ++i) {
        res = (res * 2ULL) % mod_val;
    }
    return res;
}

/* a(n) from the problem description */
seq_t a(idx_t n)
{
    /* locate the row r: minimal r with r*(r+1)/2 >= n */
    idx_t low = 1, high = 1;
    while (high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    idx_t prev = (r - 1) * r / 2;     /* number of terms before row r */
    idx_t j = n - prev;               /* 1‑based index within the row */

    /* search the j‑th coprime to (2^r - 1) */
    idx_t count = 0;
    for (idx_t candidate = 1; ; ++candidate) {
        /* compute (2^r - 1) mod candidate */
        uint64_t mod_val = (uint64_t)candidate;
        uint64_t pow_mod = modpow2_uint64((uint64_t)r, mod_val);
        uint64_t rem = (pow_mod + mod_val - 1) % mod_val;  /* (2^r - 1) mod candidate */

        /* a candidate is coprime if gcd(candidate, rem) == 1 */
        if (gcd_uint64((uint64_t)candidate, rem) == 1) {
            ++count;
            if (count == j)
                return (seq_t)candidate;
        }
    }
}
