/*
 * A224951  Leap years having 53 Saturdays and Sundays.
 * https://oeis.org/A224951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Result of A224951: Leap years having 53 Saturdays and Sundays
   The sequence repeats every 400 years with 13 occurrences per cycle.
   Formula used: a(n) = 1600 + 400*floor((n-1)/13) + offsets[(n-1)%13]
   offsets = {0,28,56,84,124,152,180,220,248,276,316,344,372}
   where offsets are the differences from 1600 in the first cycle. */

seq_t a(idx_t n)
{
    /* 0‑based index of the term in the sequence */
    idx_t idx = n - 1;
    /* number of complete 400‑year cycles before this term */
    idx_t k = idx / 13;
    /* position inside the current cycle (0..12) */
    idx_t r = idx % 13;

    /* Offsets inside the first cycle */
    static const seq_t offsets[13] = {
        (seq_t)0,
        (seq_t)28,
        (seq_t)56,
        (seq_t)84,
        (seq_t)124,
        (seq_t)152,
        (seq_t)180,
        (seq_t)220,
        (seq_t)248,
        (seq_t)276,
        (seq_t)316,
        (seq_t)344,
        (seq_t)372
    };

    /* Compute the term */
    seq_t result = (seq_t)1600;
    result += (seq_t)k * (seq_t)400;   /* add complete cycles */
    result += offsets[r];              /* add intra‑cycle offset */

    return result;
}
