/*
 * A094190  Least n-digit automorphic number.
 * https://oeis.org/A094190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Min( 5^(2^n) (mod 10^n), 16^(5^n) (mod 10^n) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"

/* Build the next automorphic number digit by digit.
   a is the current (k-1)-digit automorphic number.
   Returns the k‑digit one. */
static seq_t _next_value(seq_t a, idx_t k)
{
    /* 10^(k-1) and 10^k as seq_t */
    seq_t pow10_km1 = pow10(k - 1);
    seq_t pow10_k   = pow10(k);

    /* a^2 mod 10^k in a generic way.  For 64‑bit seq_t use
       __int128 to avoid overflow; otherwise the multiplication
       fits in the arbitrary‑precision type. */
    seq_t a2_mod;
#if defined(__GNUC__) || defined(__clang__)
    unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)a;
    a2_mod = (seq_t)(prod % (unsigned __int128)pow10_k);
#else
    a2_mod = (seq_t)((a * a) % pow10_k);
#endif

    /* Extract the coefficient of 10^(k-1) in a^2 - a. */
    seq_t diff = a2_mod - a;
    seq_t t    = (diff / pow10_km1) % 10;

    /* Solve for the next digit d: (2*a - 1)*d ≡ -t (mod 10). */
    int a_mod10 = (int)(a % 10);
    int coeff   = (2 * a_mod10 - 1) % 10;
    if (coeff < 0)
        coeff += 10;
    int target  = (10 - (int)(t % 10)) % 10;

    int d = 0;
    for (; d <= 9; ++d) {
        if ((coeff * d) % 10 == target)
            break;
    }

    return a + (seq_t)d * pow10_km1;
}

/* Main function requested by the problem. */
seq_t a(idx_t n)
{
    if (n == 1)
        return 0;                           /* a(1) = 0 */

    /* peers for the two infinite automorphic sequences */
    static idx_t  max_k = 1;
    static seq_t  a5 = 5;                    /* ends with 5 */
    static seq_t  a6 = 6;                    /* ends with 6 */

    /* extend sequences up to the requested length */
    if (max_k < n) {
        for (idx_t k = max_k + 1; k <= n; ++k) {
            a5 = _next_value(a5, k);
            a6 = _next_value(a6, k);
        }
        max_k = n;
    }

    /* n‑digit candidates from each branch */
    seq_t pow10_n    = pow10(n);
    seq_t pow10_nm1  = pow10(n - 1);
    seq_t cand5      = a5 % pow10_n;
    seq_t cand6      = a6 % pow10_n;

    bool ok5 = cand5 >= pow10_nm1;   /* true if cand5 has n digits */
    bool ok6 = cand6 >= pow10_nm1;   /* true if cand6 has n digits */

    if (ok5 && ok6)
        return (cand5 < cand6) ? cand5 : cand6;
    if (ok5)
        return cand5;
    if (ok6)
        return cand6;
    /* Fallback – one branch should always be valid for n ≥ 2. */
    return (ok6 ? cand6 : cand5);
}
