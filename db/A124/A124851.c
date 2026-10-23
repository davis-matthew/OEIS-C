/*
 * A124851  Number of base 9 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; a(n) = trace(A^n) for n ≥ 1, where A is the 9 × 9
 *            adjacency matrix A[i][j] = 1 if |i – j| ≤ 2 0 otherwise
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function uses only integer arithmetic and can be compiled
    as both C11 and C++17.  No external helper functions are required. */

static seq_t adj[9][9];         /* adjacency matrix */
static int  adj_inited = 0;

/* initialise the adjacency matrix once */
static void init_adj(void)
{
    if (adj_inited) return;
    adj_inited = 1;
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            adj[i][j] = ( (i >= j ? i-j : j-i) <= 2 ) ? 1 : 0;
}

/* multiply two 9×9 matrices: dest = a * b */
static void mul_mat(seq_t dest[9][9], const seq_t a[9][9], const seq_t b[9][9])
{
    seq_t tmp[9][9];
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
        {
            seq_t sum = 0;
            for (int k = 0; k < 9; ++k)
                sum += a[i][k] * b[k][j];
            tmp[i][j] = sum;
        }
    /* copy back to dest */
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            dest[i][j] = tmp[i][j];
}

/* exponentiate the adjacency matrix: result = adj^n */
static void pow_adj(idx_t n, seq_t result[9][9])
{
    /* initialise result to identity matrix */
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            result[i][j] = (i == j) ? 1 : 0;

    seq_t base[9][9];
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            base[i][j] = adj[i][j];

    while (n)
    {
        if (n & (idx_t)1)
            mul_mat(result, result, base);
        mul_mat(base, base, base);
        n >>= 1;
    }
}

/* main function requested by the problem statement */
seq_t a(idx_t n)
{
    init_adj();

    if (n == 0) return (seq_t)1;             /* a(0) = 1 */

    seq_t mat[9][9];
    pow_adj(n, mat);

    seq_t trace = 0;
    for (int i = 0; i < 9; ++i)
        trace += mat[i][i];

    return trace;
}
