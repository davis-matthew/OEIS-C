/*
 * A040880  Continued fraction for sqrt(911).
 * https://oeis.org/A040880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients of sqrt(911).
   a(0) = floor(sqrt(911)) = 30,
   a(n) for n ≥ 1 repeats the period 2,5,8,5,1,11,4,4,2,1,1,29,1,1,2,4,4,11,1,5,8,2,5. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t a0_seq;          /* a(0) stored as seq_t */
    static uint64_t a0_u64;       /* a(0) also stored as uint64_t for period computation */
    static seq_t period[64];      /* period elements stored as seq_t */
    static size_t period_len;     /* length of the period */

    if (!initialized) {
        /* Parameters for sqrt(911) */
        const uint64_t D = 911ULL;

        /* Compute a(0) = floor(sqrt(D)) using 64‑bit arithmetic */
        uint64_t aa = 0;
        while ((aa + 1) * (aa + 1) <= D) {
            ++aa;
        }
        a0_u64 = aa;
        a0_seq = (seq_t)aa;          /* cast to seq_t */

        /* Compute the continued‑fraction period once */
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a_curr = a0_u64;    /* current a value (uint64_t) */
        size_t idx = 0;

        while (1) {
            uint64_t m_next = d * a_curr - m;
            uint64_t d_next = (D - m_next * m_next) / d;
            uint64_t a_next = (a0_u64 + m_next) / d_next;

            period[idx++] = (seq_t)a_next;   /* store as seq_t */

            if (a_next == 2 * a0_u64)        /* period ends when a_next = 2*a0 */
                break;

            m = m_next;
            d = d_next;
            a_curr = a_next;
        }

        period_len = idx;   /* period length (24) */
        initialized = true;
    }

    if (n == 0)
        return a0_seq;

    size_t k = (size_t)((n - 1) % period_len); /* period indexing */
    return period[k];
}
