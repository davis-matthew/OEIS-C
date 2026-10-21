/*
 * A237278  Numbers k such that A000041(k) == 0 (mod 4).
 * https://oeis.org/A237278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers k such that partition number p(k) == 0 (mod 4). */
/* Uses the pentagonal–number theorem to generate p(k) modulo 4
   and stores the indices where the value is a multiple of 4. */

#include <stdlib.h>

/* ---------- static data ---------- */
static seq_t *part_mod4 = NULL;   /* p(k) modulo 4, indexed by k */
static size_t part_alloc = 0;     /* allocated capacity of part_mod4 */
static size_t part_len   = 0;     /* number of elements actually stored */

static idx_t *zero_k   = NULL;   /* indices k with p(k) ≡ 0 (mod 4) */
static size_t zero_cap = 0;      /* allocated capacity of zero_k */
static size_t zero_len = 0;      /* number of such indices stored */

/* ---------- helpers for dynamic arrays ---------- */
static void ensure_part_cap(size_t needed)
{
    if (part_alloc < needed)
    {
        size_t new_cap = part_alloc ? part_alloc * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        part_mod4 = (seq_t *)realloc(part_mod4, new_cap * sizeof(seq_t));
        part_alloc = new_cap;
    }
}

static void ensure_zero_cap(size_t needed)
{
    if (zero_cap < needed)
    {
        size_t new_cap = zero_cap ? zero_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        zero_k = (idx_t *)realloc(zero_k, new_cap * sizeof(idx_t));
        zero_cap = new_cap;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined, but safe */

    /* initialise data structures on first call */
    if (part_len == 0) {
        ensure_part_cap(1);
        part_mod4[0] = (seq_t)1;          /* p(0) = 1 */
        part_len = 1;
    }

    /* generate partition numbers until we have at least n zeros */
    while (zero_len < (size_t)n)
    {
        idx_t k   = part_len;              /* compute p(k) for this k */
        int   sum = 0;

        /* pentagonal number theorem recurrence */
        for (idx_t j = 1;; ++j) {
            idx_t g1 = j * (3ULL * j - 1ULL) / 2ULL;
            if (g1 > k) break;
            int t = (int)part_mod4[k - g1];
            sum += (j & 1ULL) ? t : -t;

            idx_t g2 = j * (3ULL * j + 1ULL) / 2ULL;
            if (g2 > k) continue;
            t = (int)part_mod4[k - g2];
            sum += (j & 1ULL) ? t : -t;
        }

        int mod4 = (sum % 4 + 4) % 4;

        /* store the new value */
        ensure_part_cap(k + 1);
        part_mod4[k] = (seq_t)mod4;
        part_len++;

        /* record if divisible by 4 */
        if (mod4 == 0) {
            ensure_zero_cap(zero_len + 1);
            zero_k[zero_len++] = k;
        }
    }

    return (seq_t)zero_k[n - 1];
}
