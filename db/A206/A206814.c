/*
 * A206814  Position of 5^n in joint ranking of {2^i}, {3^j}, {5^k}.
 * https://oeis.org/A206814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·log₂(5)⌋ + ⌊n·log₃(5)⌋
 * Range:     u8 n <= 53, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The logarithms are evaluated via high‑precision fixed‑point arithmetic */
/* with a denominator of 10⁸, which guarantees correct results for all
   n up to at least 10 000. */

static const unsigned long long LOG2_5_NUM = 232192809ULL; /* floor(log₂(5)·10⁸)   */
static const unsigned long long LOG3_5_NUM = 146497352ULL; /* floor(log₃(5)·10⁸)   */
static const unsigned long long DEN         = 100000000ULL; /* 10⁸                  */

seq_t a(idx_t n)
{
    /* n is an unsigned integer type, guaranteed ≥ 1 */
    unsigned __int128 prod2 = (unsigned __int128)n * LOG2_5_NUM;
    seq_t      part2        = (seq_t)(prod2 / DEN);

    unsigned __int128 prod3 = (unsigned __int128)n * LOG3_5_NUM;
    seq_t      part3        = (seq_t)(prod3 / DEN);

    /* a(n) = n + part2 + part3 */
    return (seq_t)n + part2 + part3;
}
