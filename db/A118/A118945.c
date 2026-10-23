/*
 * A118945  n-th (starting from the left) decimal digit of 12^n.
 * https://oeis.org/A118945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (n‑th leftmost decimal digit of 12^n) (n ≥ 1)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Base for internal big‑integer representation (fits in 32 bits). */
#define BASE 1000000000U

/* Return the n‑th leftmost decimal digit of 12^n (1‑based index). */
seq_t a(idx_t n)
{
    /* Validate minimal index.  For n < 1 the behaviour is irrelevant. */
    if (n < 1U) return (seq_t)0;

    /* Allocate an array to hold the limbs of 12^n in little‑endian order. */
    size_t cap   = 16;          /* initial capacity */
    size_t size  = 1;           /* current number of used limbs */
    uint32_t *limb = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!limb) return (seq_t)0; /* out of memory → 0 (cannot happen on test host) */
    limb[0] = 1;                /* 12^0 = 1 */

    /* Multiply by 12, n times, obtaining 12^n. */
    for (idx_t i = 1; i <= n; ++i)
    {
        uint64_t carry = 0;
        for (size_t j = 0; j < size; ++j)
        {
            uint64_t prod = (uint64_t)limb[j] * 12ULL + carry;
            limb[j] = (uint32_t)(prod % BASE);
            carry   = prod / BASE;
        }
        if (carry)
        {
            if (size == cap)                     /* grow array */
            {
                cap <<= 1;                       /* double capacity */
                uint32_t *tmp = (uint32_t *)realloc(limb, cap * sizeof(uint32_t));
                if (!tmp)
                {
                    free(limb);
                    return (seq_t)0;            /* allocation failure */
                }
                limb = tmp;
            }
            limb[size++] = (uint32_t)carry;
        }
    }

    /* Find the n‑th leftmost decimal digit without building the whole string. */
    idx_t pos = n - 1;                      /* 0‑based position from the left */
    /* Process the most significant limb first. */
    char buf[16];                           /* enough for BASE‑1 plus NUL */
    int len = snprintf(buf, sizeof(buf), "%u", limb[size - 1]); /* no leading zeros */
    if ((idx_t)len > pos)
    {
        free(limb);
        return (seq_t)(buf[pos] - '0');
    }
    pos -= len;

    /* Process remaining limbs (always padded to 9 digits). */
    for (idx_t k = size - 2; k < size; --k) /* size is uint64_t‑based, loop careful */
    {
        len = snprintf(buf, sizeof(buf), "%09u", limb[k]);
        if ((idx_t)len > pos)
        {
            free(limb);
            return (seq_t)(buf[pos] - '0');
        }
        pos -= len;
        if (k == 0) break;  /* avoid unsigned underflow */
    }

    /* Should never reach here; safety check. */
    free(limb);
    return (seq_t)0;
}
