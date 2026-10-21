/*
 * A088152  Value of n-th digit in octal representation of n^n.
 * https://oeis.org/A088152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(n^n / 8^n) mod 8
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value a(n) is the n‑th octal digit of n^n with offset 0 (a(0)=1).   */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* modular multiplication of 32‑bit little‑endian limb arrays modulo 2^m
   where m = 32 * L bits.  The result is written into dst (which must be
   zeroed before the call).  All arithmetic is performed modulo 2^m, i.e.
   the high carry bits are discarded. */
static void mul_mod(const uint32_t *a, const uint32_t *b,
                    uint32_t *dst, int L)
{
    for (int i = 0; i < L; ++i)
    {
        uint64_t carry = 0;
        for (int j = 0; j + i < L; ++j)
        {
            uint64_t t = (uint64_t)a[i] * b[j] + dst[i + j] + carry;
            dst[i + j] = (uint32_t)(t & 0xFFFFFFFFu);
            carry     = t >> 32;
        }
        /* higher carry bits are discarded modulo 2^m */
    }
}

/* compute the n‑th term of OEIS A088152 */
seq_t a(idx_t n)
{
    /* 0^0 is defined as 1 in the sequence */
    if (n == 0u)
        return (seq_t)1u;

    /* Number of bits needed for the modulus 8^(n+1) = 2^(3n+3)   */
    int bits  = 3 * (int)n + 3;
    int L     = (bits + 31) / 32;          /* number of 32‑bit limbs        */
    int shift = 3 * (int)n;                /* bits to shift right to get the digit */

    /* Small n means little work – use 32‑bit types directly if possible */
    if (bits <= 32)
    {
        /* compute n^n modulo 2^32 and then extract the character digit */
        uint64_t pow = 1;
        uint64_t base = (uint64_t)n;
        idx_t exp = n;
        while (exp)
        {
            if (exp & 1)
                pow = (pow * base) & 0xFFFFFFFFu;
            base = (base * base) & 0xFFFFFFFFu;
            exp >>= 1;
        }
        uint32_t result = (uint32_t)pow;
        return (seq_t)((result >> shift) & 7u);
    }

    /* General case: use limb arithmetic up to 2^(3n+3) bits            */
    uint32_t *res   = (uint32_t*)calloc(L, sizeof(uint32_t));  /* = 1      */
    uint32_t *base  = (uint32_t*)calloc(L, sizeof(uint32_t));  /* = n      */
    uint32_t *tmp   = (uint32_t*)calloc(L, sizeof(uint32_t));  /* scratch  */

    if (!res || !base || !tmp)
    {
        free(res); free(base); free(tmp);
        return (seq_t)0u;                 /* allocation failure */
    }

    /* initialise res = 1 */
    res[0] = 1u;

    /* initialise base = n (mod 2^(3n+3)) – n < 2^32, so only first limb matters */
    base[0] = (uint32_t)n;

    /* modular exponentiation: compute n^n (mod 2^(3n+3)) */
    idx_t exp = n;
    while (exp)
    {
        if (exp & 1u)
        {
            memset(tmp, 0, L * sizeof(uint32_t));
            mul_mod(res, base, tmp, L);
            memcpy(res, tmp, L * sizeof(uint32_t));
        }
        memset(tmp, 0, L * sizeof(uint32_t));
        mul_mod(base, base, tmp, L);
        memcpy(base, tmp, L * sizeof(uint32_t));
        exp >>= 1;
    }

    /* extract the desired digit: (res >> shift) & 7 */
    seq_t digit = 0;
    int limb_shift = shift / 32;
    int bit_shift  = shift % 32;

    if (limb_shift < L)
    {
        if (bit_shift == 0)
            digit = (seq_t)(res[limb_shift] & 7u);
        else
        {
            uint32_t low  = res[limb_shift] >> bit_shift;
            uint32_t high = (limb_shift + 1 < L)
                            ? (res[limb_shift + 1] << (32 - bit_shift))
                            : 0u;
            digit = (seq_t)((low | high) & 7u);
        }
    }

    free(res); free(base); free(tmp);
    return digit;
}
