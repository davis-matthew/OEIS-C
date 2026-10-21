/*
 * A059343  Triangle of nonzero coefficients of Hermite polynomials H_n(x) in
 *          increasing powers of x.
 * https://oeis.org/A059343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice, tabf
 * Formula:   a(n) = coefficient of the Hermite polynomial H_r(x) for the
 *            global index n in the triangle of non‑zero coefficients. The
 *            code reconstructs the row, the term offset and evaluates the
 *            combinatorial formula: a(n) = (-1)^k * (r! / (k! (r-2k)!)) *
 *            2^(r-2k)
 * Range:     i8 n <= 9, i16 n <= 25, i32 n <= 64, i64 n <= 184, i128 n <= 554,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* build‑time constants – large enough for n up to ~10⁴ */
#define MAX_ROW 20000

/* cumulative counts of non‑zero coefficients up to each row */
static idx_t cum[MAX_ROW + 1];
static int cum_inited = 0;

/* initialise the cumulative table (once) */
static void init_cum(void)
{
    if (cum_inited)
        return;
    cum[0] = 1;                     /* row 0 has one coefficient */
    for (int i = 1; i <= MAX_ROW; ++i)
        cum[i] = cum[i - 1] + ((i >> 1) + 1); /* floor(i/2)+1 */
    cum_inited = 1;
}

/* binary search to find the row that contains global index n */
static int row_of_index(idx_t n)
{
    int low = 0, high = MAX_ROW;          /* search interval [low,high] */
    while (low < high) {
        int mid = (low + high) >> 1;
        if (cum[mid] > n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;                            /* row index r */
}

/* fast exponentiation of 2^exp in arbitrary precision */
static seq_t pow2(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ----------------------------------------------------------- */
/* Main function required by the problem statement             */
/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    int row, offset, p, k;
    seq_t coeff;

    init_cum();                           /* initialise cumulative table */

    row = row_of_index(n);
    offset = (row == 0) ? 0 : (int)(n - cum[row - 1]);

    /* exponent e = r - 2k   →  p = e */
    if ((row & 1) == 0)                 /* even row -> even powers       */
        p = 2 * offset;
    else                                 /* odd row  -> odd powers        */
        p = 1 + 2 * offset;

    /* k is the number of “pairs” removed:  k = (row - p)/2  */
    k = (row - p) >> 1;

    /* ----------- compute r! / (k! (r-2k)!) ----------------- */
    coeff = 1;                           /* start with C(row,k) = 1 */

    /* compute binomial coefficient C(row,k) = row!/(k!(row-k)!) */
    for (idx_t i = 1; i <= (idx_t)k; ++i) {
        coeff *= (seq_t)(row - k + i);
        coeff /= (seq_t)i;
    }

    /* multiply by (row-k)!/(row-2k)!  ->  product (row-2k+1 .. row-k) */
    for (idx_t i = (idx_t)(row - 2 * k + 1); i <= (idx_t)(row - k); ++i)
        coeff *= (seq_t)i;

    /* multiply by the remaining power of two 2^p */
    coeff *= pow2(p);

    /* apply the sign (-1)^k */
    if ((k & 1) != 0)
        coeff = -coeff;

    return coeff;
}
