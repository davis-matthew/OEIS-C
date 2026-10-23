/*
 * A171725  6-digit numbers k such that: (a) the digits of k are from the set
 *          {1, 2, 3, 4, 5}, and (b) any digit that appears in k appears at
 *          least twice.
 * https://oeis.org/A171725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is the nth 6‑digit number composed of digits 1–5 in which *
 *            every digit that occurs appears at least twice. * * The
 *            algorithm enumerates all 5^6 = 15625 possible 6‑digit strings, *
 *            keeps those satisfying the condition, and stores them in
 *            increasing * decimal order. The array is built lazily...
 * Range:     u8 none, u16 none, u32 n <= 1405, u64 n <= 1405, u128 n <= 1405,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum number of valid terms (determined analytically). */
#define MAX_TERMS 1405

static seq_t terms[MAX_TERMS];
static size_t term_count = 0;

/* Helper: fill the terms array once. */
static void init_terms(void)
{
    if (term_count != 0) return;  /* already initialized */

    /* Loop over all base‑5 representations of length 6. */
    for (uint32_t mask = 0; mask < 15625U; ++mask) {
        int digits[6];
        uint32_t t = mask;

        /* Convert to digits 1–5 (most significant first). */
        for (int pos = 5; pos >= 0; --pos) {
            digits[pos] = (t % 5) + 1;
            t /= 5;
        }

        /* Count occurrences of each digit. */
        unsigned cnt[6] = {0};   /* indices 1–5 used */
        for (int i = 0; i < 6; ++i)
            ++cnt[digits[i]];

        /* Invalid if some non‑zero count is < 2. */
        bool ok = true;
        for (int d = 1; d <= 5; ++d) {
            if (cnt[d] && cnt[d] < 2) { ok = false; break; }
        }
        if (!ok) continue;

        /* Build the decimal value. */
        seq_t val = 0;
        for (int i = 0; i < 6; ++i)
            val = val * 10 + (seq_t)digits[i];

        /* Store the term. */
        terms[term_count++] = val;
    }
}

seq_t a(idx_t n)
{
    init_terms();                 /* ensure table is ready */

    if (n == 0 || n > term_count) return (seq_t)0;  /* out of bounds */
    return terms[n - 1];
}
