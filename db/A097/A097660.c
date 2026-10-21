/*
 * A097660  Number of zeros in decimal representation of 1001^n.
 * https://oeis.org/A097660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of zeros in decimal representation of 1001^n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const uint32_t BASE = 1000000000U;   /* 10^9 */

/* 1001^n is computed in decimal using a base‑10^9 big integer,
 * then the zeros are counted by inspecting every decimal digit. */
seq_t a(idx_t n)
{
    if (n == 0)            /* 1001^0 = 1 has no zeros */
        return 0;

    /* Allocate a little‑endian array of base‑10^9 blocks. */
    size_t cap  = 64;       /* initial capacity */
    size_t len  = 1;        /* current number of used blocks */
    uint32_t *d  = (uint32_t*)malloc(cap * sizeof(uint32_t));
    if (!d)                /* memory failure – return 0 as a safe value */
        return 0;
    d[0] = 1U;              /* start with the integer value 1 */

    /* Repeatedly multiply by 1001 – n times. */
    for (idx_t i = 0; i < n; ++i)
    {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j)
        {
            uint64_t prod = (uint64_t)d[j] * 1001ULL + carry;
            d[j] = (uint32_t)(prod % BASE);
            carry = prod / BASE;
        }
        while (carry > 0)
        {
            if (len == cap)     /* grow the storage if needed */
            {
                cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(d, cap * sizeof(uint32_t));
                if (!tmp) { free(d); return 0; }
                d = tmp;
            }
            d[len++] = (uint32_t)(carry % BASE);
            carry /= BASE;
        }
    }

    /* Count the zeros in the decimal expansion. */
    unsigned long long zero_count = 0;

    /* All blocks except the most significant one are stored
     * with exactly 9 digits (leading zeros included). */
    for (size_t j = 0; j + 1 < len; ++j)
    {
        uint32_t block = d[j];
        for (int t = 0; t < 9; ++t)
        {
            if ((block % 10) == 0) ++zero_count;
            block /= 10;
        }
    }

    /* Count zeros in the most significant block – no leading zeros. */
    uint32_t msb = d[len - 1];
    if (msb == 0)          /* should not happen, but handle safely */
    {
        ++zero_count;
    }
    else
    {
        while (msb > 0)
        {
            if ((msb % 10) == 0) ++zero_count;
            msb /= 10;
        }
    }

    free(d);
    return (seq_t)zero_count;
}
