/*
 * A138891  Non-Motzkin numbers.
 * https://oeis.org/A138891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th nonnegative integer that is NOT a Motzkin number
 *            (A001006). The Motzkin numbers satisfy the recurrence: M(0) = 1,
 *            M(1) = 1, M(k) = ((2*k+1)*M(k-1) + 3*(k-1)*M(k-2)) / (k+2) for k
 *            ≥ 2 We generate Motzkin numbers as needed and count the integers
 *            that are not in this set
 * Range:     u8 n <= 17, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Static storage for the computed Motzkin numbers.  The array is large
   enough to hold all Motzkin numbers up to the values that will be
   required for n up to about 10⁴ (only the first few dozen are needed). */
static seq_t motzkin[64] = { (seq_t)1, (seq_t)1 };
static size_t motz_len = 2;
static bool motz_initialized = false;

/* Compute the next Motzkin number using the recurrence relation.  The
   intermediate arithmetic is performed in seq_t, which is assumed to be
   large enough to hold the result for the indices that will be generated. */
static void generate_next_motzkin(void)
{
    idx_t k = (idx_t)motz_len;        /* next index to be computed */
    seq_t m_k1 = motzkin[k - 1];      /* M(k-1) */
    seq_t m_k2 = motzkin[k - 2];      /* M(k-2) */

    /* ((2*k+1)*M(k-1) + 3*(k-1)*M(k-2)) / (k+2) */
    seq_t term1 = (seq_t)(2 * k + 1) * m_k1;
    seq_t term2 = (seq_t)3 * (k - 1) * m_k2;
    seq_t numerator = term1 + term2;
    seq_t denominator = (seq_t)k + (seq_t)2;
    seq_t next = numerator / denominator;   /* exact division */

    /* Append the new term.  The array is guaranteed to be large enough. */
    motzkin[motz_len++] = next;
}

/* Returns the n-th nonnegative integer that is not a Motzkin number
   (n = 1 corresponds to the first term in the OEIS listing). */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* The OEIS offset is 1; behaviour for n < 1 is undefined,
           but we return 0 for safety. */
        return (seq_t)0;
    }

    if (!motz_initialized) {
        motz_len = 2;
        motzkin[0] = (seq_t)1;
        motzkin[1] = (seq_t)1;
        motz_initialized = true;
    }

    idx_t candidate = 0;          /* current integer being examined */
    idx_t count = 0;              /* how many non‑Motzkin numbers seen */

    for (;;) {
        /* Ensure the Motzkin array contains all values up to at least
           'candidate' so that we can test membership efficiently. */
        while (motz_len > 0 && motzkin[motz_len - 1] < (seq_t)candidate) {
            generate_next_motzkin();
        }

        /* Check if 'candidate' is a Motzkin number. */
        bool is_motzkin = false;
        for (size_t i = 0; i < motz_len; ++i) {
            if (motzkin[i] == (seq_t)candidate) {
                is_motzkin = true;
                break;
            }
            if (motzkin[i] > (seq_t)candidate)
                break;
        }

        if (!is_motzkin) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }

        ++candidate;
    }
}
