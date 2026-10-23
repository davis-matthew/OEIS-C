/*
 * A138114  Triangle read by rows: row n lists the first n digits of the
 *          decimal expansion of Pi.
 * https://oeis.org/A138114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, base
 * Formula:   a(n) = π‑digit at position n − T_{r-1} where r =
 *            ceil((√(8·n+1)−1)/2) and T_k = k·(k+1)/2
 * Range:     u8 n <= 44, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const char PI_DIGITS[] =
    "3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909148648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362420656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923460090225991";

static const idx_t PI_DIGITS_LEN = sizeof(PI_DIGITS) - 1;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1, value for 0 unused */

    /* Convert n to seq_t once for comparisons in the binary search. */
    seq_t n_seq = (seq_t)n;

    /* Binary search for the row r such that T_{r-1} < n <= T_r. */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t tri = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (tri >= n_seq)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                     /* row number */

    /* Position within the row (1‑based). */
    idx_t tri_prev = (r - 1) * r / 2;
    idx_t k = n - tri_prev;           /* 1‑based index inside the row */

    /* 0‑based index into PI_DIGITS. */
    idx_t digit_index = k - 1;

    /* Guard against an unexpected out of bounds (should not happen). */
    if (digit_index >= PI_DIGITS_LEN)
        return 0;

    return (seq_t)(PI_DIGITS[digit_index] - '0');
}
