/*
 * A221420  Number of 2 X n arrays of occupancy after each element stays put
 *          or moves to some horizontal or antidiagonal neighbor, with every
 *          occupancy equal to zero or two.
 * https://oeis.org/A221420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 2*a(n-4) for n > 6 Base values:
 *            a(1)=0, a(2)=4, a(3)=10, a(4)=29, a(5)=84, a(6)=242
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small table of the first six terms (index 0 unused). */
    static const seq_t base[7] = {0, 0, 4, 10, 29, 84, 242};

    /* For the offsets covered by the table, return directly. */
    if (n <= 6)
        return base[n];

    /* Allocate a temporary array to compute up to n. */
#ifdef __cplusplus
    seq_t *arr = new seq_t[n + 1];
#else
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif

    /* Initialise the first six elements. */
    arr[1] = base[1];
    arr[2] = base[2];
    arr[3] = base[3];
    arr[4] = base[4];
    arr[5] = base[5];
    arr[6] = base[6];

    /* Apply the recurrence for all subsequent indices. */
    for (idx_t i = 7; i <= n; ++i)
        arr[i] = 4 * arr[i - 1] - 3 * arr[i - 2] - 2 * arr[i - 4];

    /* Retrieve the requested value. */
    seq_t result = arr[n];

    /* Free the temporary storage. */
#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif

    return result;
}
