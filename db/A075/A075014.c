/*
 * A075014  Smallest k such that the concatenation k, k-1 is divisible by n;
 *          or 0 if no such number exists.
 * https://oeis.org/A075014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 802, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   extgcd_int64, gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/extgcd_int64.h"
#include "../utils/gcd_u64.h"
/* A075014: a(n) = smallest k>0 such that the decimal concatenation k,(k-1)
   is divisible by n; 0 if no such k exists.  The calculation uses the
   congruence k·(10^d+1) ≡ 1 (mod n) where d = number of decimal digits of k-1. */

/* --------------------------------------------------------------------- */
/* Helper: gcd for uint64_t                                               */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Helper: extended Euclidean algorithm  (works for a,b < 2^63)          */

/* --------------------------------------------------------------------- */
/* Modular inverse of a modulo mod (mod up to 10000).                      */
/* returns 0 if inverse does not exist (gcd != 1).                         */
/* --------------------------------------------------------------------- */
static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
{
    int64_t x, y, g;
    extgcd_int64((int64_t)a, (int64_t)mod, &x, &y, &g);
    if (g != 1)
        return 0;
    int64_t inv = x % (int64_t)mod;
    if (inv < 0)
        inv += mod;
    return (uint64_t)inv;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n is always >= 1 in OEIS.  Handle n==1 specially.                */
    if (n == 1)
        return (seq_t)1;

    /* k = 1 works exactly when 10 is a multiple of n.                   */
    if (10 % n == 0)
        return (seq_t)1;

    /* The algorithm checks increasing digit counts d of k-1.              */
    const int MAX_DIGITS = 25;            /* sufficient for n <= 10^4    */

    uint64_t pow10_mod = 1 % (uint64_t)n;    /* 10^0 mod n                */
    unsigned __int128 pow10_val = 1;         /* 10^0                      */
    unsigned __int128 pow10_prev = 1;        /* 10^(d-1)                  */

    for (int d = 1; d <= MAX_DIGITS; ++d) {
        /* advance to 10^d */
        pow10_val   *= (unsigned __int128)10;
        pow10_mod   = (pow10_mod * 10) % (uint64_t)n;
        pow10_prev  = pow10_val / (unsigned __int128)10;  /* 10^(d-1)      */

        /* A = 10^d + 1  (full integer), but we only need it modulo n.   */
        uint64_t A_mod = (pow10_mod + 1U) % (uint64_t)n;

        /* Inverse exists only if gcd(A_mod, n) == 1.                       */
        if (gcd_u64(A_mod, (uint64_t)n) != 1)
            continue;

        uint64_t inv = modinv_uint64(A_mod, (uint64_t)n);
        if (inv == 0)              /* safety check */
            continue;

        /* k ≡ inv (mod n)  ;  use the least positive residue.           */
        uint64_t k0 = inv % (uint64_t)n;
        if (k0 == 0)
            k0 = (uint64_t)n;

        /* Bounds on k such that k-1 has exactly d digits:                */
        unsigned __int128 lower = pow10_prev + 1;   /* 10^(d-1)+1          */
        unsigned __int128 upper = pow10_val;        /* 10^d                 */

        /* Find the smallest k >= lower with k ≡ k0 (mod n).                */
        unsigned __int128 k_candidate;
        if (k0 >= (uint64_t)lower) {
            k_candidate = k0;
        } else {
            unsigned __int128 diff = lower - (unsigned __int128)k0;
            unsigned __int128 t    = (diff + (uint64_t)n - 1) / (uint64_t)n;
            k_candidate = (unsigned __int128)k0 + t * (uint64_t)n;
        }

        if (k_candidate <= upper)
            return (seq_t)k_candidate;
        /* else continue with next digit length */
    }

    /* No suitable k found within the searched digit range.            */
    return (seq_t)0;
}
