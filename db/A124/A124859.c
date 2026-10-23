/*
 * A124859  Multiplicative with p^e -> primorial(e), p prime and e > 0.
 * https://oeis.org/A124859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} primorial(e) where primorial(e) = product of
 *            the first e primes (p_1…p_e)
 * Range:     u8 n <= 31, u16 n <= 127, u32 n <= 1023, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* list of the first 64 primes – enough for any 64‑bit n (exponent ≤ 64) */
static const uint32_t first_primes[64] = {
    2,   3,   5,   7,  11,  13,  17,  19,  23,  29,
   31,  37,  41,  43,  47,  53,  59,  61,  67,  71,
   73,  79,  83,  89,  97, 101, 103, 107, 109, 113,
  127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
  179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
  233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
  283, 293, 307, 311
};

/* cached primorial values: primorial_cache[e] = product of first e primes   */
static seq_t primorial_cache[65] = {0};
static int max_primorial_cached = 0;    /* highest e for which primorial_cache[e] is valid */

/* initialise primorial_cache[0] and primorial_cache[1] */
static void init_primorial(void)
{
    if (max_primorial_cached != 0) return;
    primorial_cache[0] = 1;           /* by convention */
    primorial_cache[1] = 2;           /* product of first prime */
    max_primorial_cached = 1;
}

/* ensure primorial_cache[e] is computed; e ≤ 64 */
static void ensure_primorial_cached(int e)
{
    if (e <= max_primorial_cached) return;
    for (int i = max_primorial_cached + 1; i <= e; ++i) {
        primorial_cache[i] = primorial_cache[i-1] * (seq_t)first_primes[i-1];
    }
    max_primorial_cached = e;
}

/* multiplicative sequence A124859 */
seq_t a(idx_t n)
{
    if (n == 1) return 1;

    init_primorial();                          /* initialise cache */

    idx_t temp = n;                            /* value to factorise */
    seq_t result = 1;

    /* trial division – fast enough for n up to about 10 000 (the test range) */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            ensure_primorial_cached(exp);
            result *= primorial_cache[exp];
        }
    }

    /* remaining prime factor with exponent 1, if any */
    if (temp > 1) {
        ensure_primorial_cached(1);
        result *= primorial_cache[1];
    }

    return result;
}
